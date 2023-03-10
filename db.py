import sqlite3
from sqlite3 import Error
from dataclasses import dataclass
import typing
import logging, coloredlogs

# logging.basicConfig(level=logging.INFO)
coloredlogs.DEFAULT_LEVEL_STYLES['info'] = {'bold' : False, 'color': 'blue'}
coloredlogs.install(level='INFO',
                    fmt='%(asctime)s %(levelname)s %(message)s',
                    datefmt='%Y-%m-%d %H:%M:%S')

# class DotDict(dict):
#     """dot.notation access to dictionary attributes"""
#     __getattr__ = dict.get
#     __setattr__ = dict.__setitem__
#     __delattr__ = dict.__delitem__

        
@dataclass
class Item:
    def get(self):
        vals = {}
        for field_name in typing.get_type_hints(self).items():
            # print(field_name)
            val = getattr(self, field_name[0])
            vals[field_name[0]] = val
        return vals

@dataclass
class User(Item):
    id: int = None
    name: str = None
    password: str = None
    email: str = None

@dataclass
class Project(Item):
    id: int = None
    name: str = None
    content: str = None

@dataclass
class Video(Item):
    id: int = None
    path: str = None

@dataclass
class ProjectVideo(Item):
    project_id: int = None
    video_id: int = None

@dataclass
class ProjectUser(Item):
    project_id: int = None
    user_id: int = None


class DataBase():
    def __init__(self, db_path):
        self.db_path = db_path
        self.conn = None
    
    def connect(self):
        """Creates a database connection to the SQLite database specified by db_file

        Args:
            db_file (str): database file path
            return: Connection object or None
        """
        try:
            self.conn = sqlite3.connect(self.db_path)
        except Error as e:
            print(e)

class Table():
    def __init__(self, name, conn) -> None:
        self.name = name
        self.conn = conn

    def _get_str_keys(self, dct: dict, between: str):
        logging.debug(dct)
        s = between.join([str(key) for key in dct.keys()])
        # s = s[:-1*len(between)]
        return s
    
    def insert(self, item):
        dct = item.get()
        logging.info(f'Table: {self.name} |\tINSERT: {dct}')
        sql = f'''INSERT INTO {self.name}({self._get_str_keys(dct, ", ")})
        VALUES (''' + '?, ' * len(dct.keys())
        
        sql = sql[:-2] + ')'
        logging.debug(sql.replace('\n', ' '))
        try:
            self.conn.execute(sql, [str(val) for val in dct.values()])
        except sqlite3.IntegrityError as e:
            logging.error(e)
        self.conn.commit()
        
    def select(self, columns, item):
        dct = item.get()
        dct = {i:dct[i] for i in dct if dct[i] != None}
                
        sql = f'''SELECT {columns} FROM {self.name}'''
        if len(dct) != 0:
            sql += f''' WHERE {self._get_str_keys(dct, "=?, ")}=?'''
        logging.info(sql.replace('\n', ' '))
        
        cur = self.conn.cursor()
        cur.execute(sql, [str(val) for val in dct.values()])
        rows = cur.fetchall()
        return rows
        
    def create(self, create_table_sql):
        
        """Creates a table from create_table_sql statement

        Args:
            conn (sqlite.connect): Connection object
            create_table_sql (str): a CREATE TABLE statement
        """
        try:
            self.conn.execute(create_table_sql)
        except Error as e:
            logging.error(e)

db_path = r"mydb.db"
database = DataBase(db_path)
database.connect()

create_tables = {}
create_tables["users"] = """
CREATE TABLE if not EXISTS users (
    id integer PRIMARY KEY,
    name text NOT NULL,
    password text NOT NULL,
    email text NOT NULL
); """
create_tables["projects"] = """
CREATE TABLE if not EXISTS projects (
    id integer PRIMARY KEY,
    name text NOT NULL,
    content text
); """
create_tables["videos"] = """
CREATE TABLE if not EXISTS videos (
    id integer PRIMARY KEY,
    path text NOT NULL
); """
create_tables["project_videos"] = """
CREATE TABLE if not EXISTS project_videos (
    project_id integer,
    video_id integer,
    unique (project_id, video_id)
); """
create_tables["project_users"] = """
CREATE TABLE if not EXISTS project_users (
    project_id integer,
    user_id integer,
    unique (project_id, user_id)
); """


tables : typing.Dict[str, Table] = {}

for create in create_tables:
    tables[create] = Table(create, database.conn)
    tables[create].create(create_tables[create])

if __name__ =="__main__":
    tables['project_users'].insert(ProjectUser(1,2))
    tables['project_users'].insert(ProjectUser(1,2))
    tables['project_users'].insert(ProjectUser(1,2))
    tables['project_users'].insert(ProjectUser(3,4))
    tables['project_users'].insert(ProjectUser(4,3))
    tables['project_users'].insert(ProjectUser(4,4))
    tables['users'].insert(User(3, "Noa", "123", "noaklein.email@gmail.com"))
    tables['users'].insert(User(1, "ads", "123", "noaklein.email@gmail.com"))
    tables['users'].insert(User(4, "as", "123", "noaklein.email@gmail.com"))
    tables['users'].insert(User(5, "he", "12f3", "noaklein.email@gmail.com"))
    print(tables['users'].select("*", User(None, None, "123", None)))
    print(tables['users'].select("name", User(None, None, "123", None)))
    print(type(tables['users'].select("*", User(3, None, None, None))))
    print(tables['users'].select("*", User(id=3)))
    print(tables['users'].select("*", User(None, None, None, None)))
    print(tables['users'].select("*", User(None, None, None, "aaa")))
    print(tables['users'].select('MAX(id)', User())[0][0])
    