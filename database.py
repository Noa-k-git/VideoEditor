import sqlite3
from sqlite3 import Error

from dataclasses import dataclass
from abc import ABC, abstractclassmethod
import typing
from typing import List, Tuple
import logging, coloredlogs

from hashlib import md5

# logging.basicConfig(level=logging.INFO)
coloredlogs.DEFAULT_LEVEL_STYLES['info'] = {'bold': False, 'color': 'blue'}
coloredlogs.install(level='DEBUG',
                    fmt='%(asctime)s %(levelname)s %(message)s',
                    datefmt='%Y-%m-%d %H:%M:%S')


# class DotDict(dict):
#     """dot.notation access to dictionary attributes"""
#     __getattr__ = dict.get
#     __setattr__ = dict.__setitem__
#     __delattr__ = dict.__delitem__
def convert_to_int(val):
    if val is None:
        return -1
    return int(val)


@dataclass
class Item(ABC):
    def get_str_keys(self, between: str, before: str = ""):
        s = between.join([before + key for key in self.properties])
        # s = s[:-1*len(between)]
        return s

    def __iter__(self):
        for key, _ in self.__annotations__.items():
            value = getattr(self, key)
            if value != None:
                yield key, str(value)

    @property
    def properties(self) -> List[str]:
        return list(dict(self).keys())

    @property
    def data(self) -> List[str]:
        return list(dict(self).values())


@dataclass
class User(Item):
    id_: int = None
    name: str = None
    email: str = None
    password: bytes = None


@dataclass
class Project(Item):
    id_: int = None
    name: str = None
    admin_id: int = None
    content: str = None
    approved: str = None


@dataclass
class Video(Item):
    id_: int = None
    path: str = None


@dataclass
class ProjectVideo(Item):
    project_id: int = None
    video_id: int = None


@dataclass
class ProjectUser(Item):
    user_id: int = None
    project_id: int = None


class Table():
    def __init__(self, name, columns=[]) -> None:
        self.name = name
        self.columns = columns

    def __getitem__(self, column_names):
        if isinstance(column_names, str):
            column_names = [column_names]
        return Table(self.name, [c for c in column_names if c in self.columns])

    def create(self, conn, columns: dict[str, str]):
        """Creates a table from create_table_sql statement

        Args:
            conn (sqlite.connect): Connection object
            create_table_sql (str): a CREATE TABLE statement
        """
        self.columns = columns.keys()
        try:
            unique = f",\n\tunique({columns.pop('unique')})"
        except KeyError:
            unique = ''
        try:
            idx_cols = columns.pop('INDEX')
            idx_name = f"{self.name}_{idx_cols.replace(' ', '_').replace(',', '_')}_idx"
            index = f"\nCREATE INDEX {idx_name} ON {self.name} ({idx_cols});"
        except KeyError:
            index = ''
        sql = f"""CREATE TABLE if not EXISTS {self.name} (
            {', '.join([f'{key} {value}' for key, value in columns.items()])}{unique}
        );"""
        logging.info(sql)
        try:
            conn.execute(sql)
            conn.execute(index)
        except Error as e:
            logging.error(e)

    def _where_item(self, item: Item):
        if len(dict(item)) != 0:
            return f"WHERE {item.get_str_keys('=? AND ')}=?"
        return ''

    def insert(self, conn, item: Item):
        if 'id_' in item.properties:
            item.id = convert_to_int(self.select(conn, 'MAX(id_)', User())[0][0]) + 1
            logging.debug(f"the new id for table {self.name} is {item.id} ")
        s = []
        logging.info(f'Table: {self.name} |\tINSERT: {item}')
        sql = f'''INSERT INTO {self.name}({item.get_str_keys(", ")})
        VALUES (''' + '?, ' * len(item.properties)

        sql = sql[:-2] + ')'
        logging.debug((sql.replace('\n', ' '), item.data))
        try:
            conn.execute(sql, list(item.data))
        except sqlite3.IntegrityError as e:
            logging.error(e)
        conn.commit()

    def delete(self, conn, item: Item):
        sql = f'''DELETE FROM {self.name} 
        {self._where_item(item)}'''
        logging.info((sql.replace('\n', ' '), item.data))

        cur = conn.cursor()
        cur.execute(sql, item.data)

    def update(self, conn, new_item: Item, where_item):
        sql = f''' UPDATE {self.name} SET 
        {new_item.get_str_keys('=?, ')}=?
        {self._where_item(where_item)}'''
        logging.info((sql.replace('\n', ' '), new_item.data, where_item.data))
        cur = conn.cursor()
        cur.execute(sql, new_item.data + where_item.data)

    def select(self, conn, columns: List[str], item: Item):
        if isinstance(columns, str):
            columns = [columns]

        sql = f'''SELECT {', '.join(columns)} FROM {self.name} {self._where_item(item)}'''
        logging.info((sql.replace('\n', ' '), item.data))

        cur = conn.cursor()
        cur.execute(sql, item.data)
        rows = cur.fetchall()
        return rows


class DataBase():
    def __init__(self, db_path):
        self.db_path = db_path
        self.tables = []
        self._create_tables()  # add tables to self.tables

    def _create_tables(self):
        """Creates the tables for the database
        """
        conn = self.connect()
        create_tables = {}
        create_tables["users"] = {'id_': 'integer PRIMARY KEY',
                                  'name': 'text NOT NULL',
                                  'password': 'blob NOT NULL',
                                  'email': 'text NOT NULL',
                                  'unique': 'email'}
        #   'INDEX' : 'email'}

        create_tables["projects"] = {'id_': 'integer PRIMARY KEY',
                                     'name': 'text NOT NULL ',
                                     'admin_id': 'int',
                                     'content': 'text',
                                     'approved': 'text',
                                     'unique': 'name, admin_id'}

        create_tables["videos"] = {'id_': 'integer PRIMARY KEY',
                                   'path': 'text NOT NULL'}

        create_tables["project_videos"] = {'project_id': 'integer',
                                           'video_id': 'integer',
                                           'unique': 'project_id, video_id'}

        create_tables["project_users"] = {'project_id': 'integer',
                                          'user_id': 'integer',
                                          'unique': 'user_id, project_id'}
        #   'INDEX': 'user_id'}

        self.tables: typing.Dict[str, Table] = {}

        for name in create_tables:
            self.tables[name] = Table(name)
            self.tables[name].create(conn, create_tables[name])

    def connect(self):
        """Creates a database connection to the SQLite database specified by self.db_path
        """
        try:
            conn = sqlite3.connect(self.db_path)
            return conn
        except Error as e:
            print(e)

    def insert(self, table: Table, item: Item):
        conn = self.connect()
        return self.tables[table.name].insert(conn, item)

    def update(self, table: Table, new_item: Item, where_item: Item):
        conn = self.connect()
        self.tables[table.name].update(conn, new_item, where_item)
        conn.commit()
        conn.close()

    def delete(self, table: Table, item: Item):
        conn = self.connect()
        self.tables[table.name].delete(conn, item)
        conn.commit()

    def select(self, columns: Table, item: Item):
        conn = self.connect()
        return self.tables[columns.name].select(conn, columns.columns, item)

    def join_select(self, og: Table,
                    func='',
                    columns: List[Table] = [],
                    join: List[dict[Table: List[Table]]] or dict[Table: List[Table]] = [],
                    where: List[Tuple[Table, Item]] = None) -> List:
        """The function return the select result, if the select has join in it

        Args:
            og (Table): The base (original) table, the first table in the from expression, all joins are referring to it.
            func (str, optional): Everything except table, column. Goes right after the select statement. 
                                examples: *, MAX(col_name), MIN(col_name) ext. Defaults to ''.
            columns (List[Table], optional): A list of all the wanted columns content. 
                                            It is a list of Table and in every Table there are as many columns as passed. Defaults to [].
            join (List[dict[Table: List[Table]]] / dict[Table: List[Table]] , optional): The inner join connection. The key for each element is the new Table joined,
                                                            and the value of each element is a list of Tables with columns to match to the joined table. Pair Them by order.
                                                            There should be the same number of columns for the key and value. Defaults to [].
            where (List[Tuple[Table, Item]], optional): Parameter for the where statement. It is a list that contains tuples of table and item to match its values to the table.
                                                        all tables in the where statement should be defined in the join. Defaults to None.

        Returns:
            List: The result from the select.
        """

        def join_list(source: Table,
                      targets: List[Table]) -> dict[str:str]:
            """This function receives a table and a list of other tables to join to

            Raises:
                TypeError: if the number of columns of the source table doesn't match the number of target columns. 

            Returns:
                dict[str:str]: the two columns to compare
            """
            t_cols = []
            if isinstance(targets, Table):
                targets = list(targets)
            for table in targets:
                t_cols += [table.name + '.' + col for col in table.columns]
            if len(t_cols) == len(source.columns):
                return list(zip([source.name + '.' + col for col in source.columns], t_cols))
            raise TypeError("Not matching columns")

        if not isinstance(columns, list):
            columns = [columns, ]
        if not isinstance(join, list):
            join = [join]
        if not isinstance(where, list):
            where = [where]
        conn = self.connect()  # connection to the database
        if func:
            func += ', '
        sql = f'''SELECT {func}{", ".join([f'{table.name}.{column}' for table in columns for column in table.columns])} '''

        sql += f'''FROM {'(' * len(join)}{og.name} '''
        if join:
            for d in join:
                for source, targets in d.items():
                    sql += f"INNER JOIN {source.name} ON "
                    l_join = join_list(source, targets)
                    sql += ' AND '.join([' = '.join(pair) for pair in l_join])
                    sql += ')'

        vars = []
        if where:
            sql += f'\nWHERE '
            for idx, where_part in enumerate(where):
                sql += f'{where_part[1].get_str_keys("=? AND ", f"{where_part[0].name}.")}=? '
                vars += where_part[1].data
                if idx != len(where) - 1:
                    sql += ' OR '
        sql += ';'

        logging.info((sql.replace('\n', ' ')))
        cur = conn.cursor()
        cur.execute(sql, vars)
        rows = cur.fetchall()
        return rows


db_path = r"mydb.db"
import os

os.remove(db_path)

if __name__ == "__main__":
    database = DataBase(db_path)
    conn = database.connect()
    sql = 'DELETE FROM project_users          WHERE user_id=? AND project_id=?'
    cur = conn.cursor()
    cur.execute(sql, (2, 2))
    conn.commit()
    # database.insert('project_users', ProjectUser(project_id=1,user_id=2))
    # database.insert('project_users', ProjectUser(project_id=3,user_id=2))
    # database.insert('project_users', ProjectUser(2,2))
    # database.insert('project_users', ProjectUser(0, 2))
    # database.insert('users', User(1,'a','a','a'))
    # database.insert('users', User(2,'b','b','b'))
    # database.insert('users', User(3,'c','c','c'))
    # database.insert('projects', Project(1, 'p1', ''))
    # database.insert('projects', Project(2, 'p2', ''))
    # print(database.select(database.tables['users']['id'], User(name='a')))
    # print(database.join_select(og=database.tables['users'], 
    #         columns=[database.tables['users']['name', 'password'],
    #                                 database.tables['projects']['name']],
    #         join=[{database.tables['project_users']['user_id']:[database.tables['users']['id'],],},
    #               {database.tables['projects']['id']:[database.tables['project_users']['project_id'],]}],
    #         ))
#     conn = database.connect()
#     sql = """SELECT 
#   projects.id, 
#   projects.name AS project_name, 
#   users.id AS user_id, 
#   users.name AS user_name, 
#   users.email AS user_email

# FROM project_users 
#   INNER JOIN users ON users.id = project_users.user_id 
#   INNER JOIN projects ON projects.id = project_users.project_id 
# WHERE 
#   project_users.project_id = 1 OR 
#   project_users.project_id = 2 OR 
#   project_users.project_id = 3;"""
#     cur = conn.cursor()
#     cur.execute(sql)
#     rows = cur.fetchall()
#     print(rows)
