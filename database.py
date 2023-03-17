import sqlite3
from sqlite3 import Error

from dataclasses import dataclass
from abc import ABC, abstractclassmethod
import typing
import logging, coloredlogs
from hashlib import md5
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
def convert_to_int(val):
    if val is None:
        return -1
    return int(val)
@dataclass
class Item(ABC):
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
    email: str = None
    hashed_password: bytes = None
    

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

class Table():
    def __init__(self, name, conn) -> None:
        self.name = name
        self.conn = conn
        self._id = False # true if has an id column, false otherwise

    def _get_str_keys(self, dct: dict, between: str):
        logging.debug(dct)
        s = between.join([str(key) for key in dct.keys()])
        # s = s[:-1*len(between)]
        return s
    
    def create(self, create_table_sql):
        
        """Creates a table from create_table_sql statement

        Args:
            conn (sqlite.connect): Connection object
            create_table_sql (str): a CREATE TABLE statement
        """
        try:
            self.conn.execute(create_table_sql)
            # self._id = self.conn.execute(f"""PRAGMA table_info('{self.name}');""").fetchall()[0][1] == 'id'
        except Error as e:
            logging.error(e)
            
    def insert(self, item: Item):
        dct = item.get()
        if 'id' in dct.keys():
            print(database.tables['users'].select('MAX(id)', User())[0][0])
            dct['id'] = convert_to_int(self.select('MAX(id)', User())[0][0]) + 1
            logging.debug(f"the new id for table {self.name} is {dct['id']} ")
        
        logging.info(f'Table: {self.name} |\tINSERT: {dct}')
        sql = f'''INSERT INTO {self.name}({self._get_str_keys(dct, ", ")})
        VALUES (''' + '?, ' * len(dct.keys())
        
        sql = sql[:-2] + ')'
        logging.debug((sql.replace('\n', ' '), dct.values()))
        try:
            self.conn.execute(sql, [str(val) for val in dct.values()])
        except sqlite3.IntegrityError as e:
            logging.error(e)
        self.conn.commit()
        
    def select(self, columns, item:Item):
        dct = item.get()
        dct = {i:dct[i] for i in dct if dct[i] != None}
                
        sql = f'''SELECT {columns} FROM {self.name}'''
        if len(dct) != 0:
            sql += f''' WHERE {self._get_str_keys(dct, "=?, ")}=?'''
        logging.info((sql.replace('\n', ' '), dct.values()))
        
        cur = self.conn.cursor()
        cur.execute(sql, [str(val) for val in dct.values()])
        rows = cur.fetchall()
        return rows

class DataBase():
    def __init__(self, db_path):
        self.db_path = db_path
        self.conn = None
        self.connect() # changes self.conn
        self.tables = []
        self._create_tables() # add tables to self.tables
    
    def connect(self):
        """Creates a database connection to the SQLite database specified by self.db_path
        """
        try:
            self.conn = sqlite3.connect(self.db_path)
        except Error as e:
            print(e)
            
    def _create_tables(self):
        """Creates the tables for the database
        """
        create_tables = {}
        create_tables["users"] = """
        CREATE TABLE if not EXISTS users (
            id integer PRIMARY KEY,
            name text NOT NULL,
            password blob NOT NULL,
            email text NOT NULL,
            unique (email)
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
            admin integer,
            unique (project_id, user_id)
        ); """

        self.tables : typing.Dict[str, Table] = {}

        for name in create_tables:
            self.tables[name] = Table(name, self.conn)
            self.tables[name].create(create_tables[name])


        

db_path = r"mydb.db"
database = DataBase(db_path)
# database.connect()

# create_tables = {}
# create_tables["users"] = """
# CREATE TABLE if not EXISTS users (
#     id integer PRIMARY KEY,
#     name text NOT NULL,
#     password text NOT NULL,
#     email text NOT NULL
# ); """
# create_tables["projects"] = """
# CREATE TABLE if not EXISTS projects (
#     id integer PRIMARY KEY,
#     name text NOT NULL,
#     content text
# ); """
# create_tables["videos"] = """
# CREATE TABLE if not EXISTS videos (
#     id integer PRIMARY KEY,
#     path text NOT NULL
# ); """
# create_tables["project_videos"] = """
# CREATE TABLE if not EXISTS project_videos (
#     project_id integer,
#     video_id integer,
#     unique (project_id, video_id)
# ); """
# create_tables["project_users"] = """
# CREATE TABLE if not EXISTS project_users (
#     project_id integer,
#     user_id integer,
#     admin integer,
#     unique (project_id, user_id)
# ); """


# tables : typing.Dict[str, Table] = {}

# for create in create_tables:
#     tables[create] = Table(create, database.conn)
#     tables[create].create(create_tables[create])

if __name__ =="__main__":
    # print(Item().get())
    database.tables['project_users'].insert(ProjectUser(1,2))
    database.tables['project_users'].insert(ProjectUser(1,2))
    database.tables['project_users'].insert(ProjectUser(1,2))
    database.tables['project_users'].insert(ProjectUser(3,4))
    database.tables['project_users'].insert(ProjectUser(4,3))
    database.tables['project_users'].insert(ProjectUser(4,4))
    database.tables['users'].insert(User(3, "Noa", "123", "noalein.emil@gmail.com"))
    database.tables['users'].insert(User(1, "ads", "123", "noaklein.mailgmail.cm"))
    database.tables['users'].insert(User(4, "as", "123", "noklein.email@gmai.com"))
    database.tables['users'].insert(User(5, "he", "12f3", "noakein.email@mail.co"))
    print(database.tables['users'].select("*", User(None, None, "123", None)))
    print(database.tables['users'].select("name", User(None, None, "123", None)))
    print(type(database.tables['users'].select("*", User(3, None, None, None))))
    print(database.tables['users'].select("*", User(id=3)))
    print(database.tables['users'].select("*", User(None, None, None, None)))
    print(database.tables['users'].select("*", User(None, None, None, "aaa")))
    print(database.tables['users'].select('MAX(id)', User()))
    print(database.tables['users'].select('MAX(id)', User())[0][0])
    