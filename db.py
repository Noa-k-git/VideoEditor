import sqlite3
from sqlite3 import Error


def create_connection(db_file):
    """Creates a database connection to the SQLite database specified by db_file

    Args:
        db_file (str): database file path
        return: Connection object or None
    """
    conn = None
    try:
        conn = sqlite3.connect(db_file)
        return conn
    except Error as e:
        print(e)
    return conn

def create_table(conn, create_table_sql):
    """Creates a table from create_table_sql statement

    Args:
        conn (sqlite.connect): Connection obejct
        create_table_sql (str): a CREATE TABLE statement
    """
    try:
        c = conn.cursor()
        c.execute(create_table_sql)
    except Error as e:
        print(e)

def append_to_table(conn, tabel_name, args_lst):
    sql = f"""INSERT INTO {table} ()"""

database = r"mydb.db"

create_tables = {}
create_tables["sql_create_users_table"] = """
CREATE TABLE if not EXISTS users (
    id integer PRIMARY KEY,
    name text NOT NULL,
    password text NOT NULL,
    email text NOT NULL
); """
create_tables["sql_create_project_table"] = """
CREATE TABLE if not EXISTS projects (
    id integer PRIMARY KEY,
    name text NOT NULL,
    project text
); """
create_tables["sql_create_videos_table"] = """
CREATE TABLE if not EXISTS videos (
    id integer PRIMARY KEY,
    path text NOT NULL
); """
create_tables["sql_create_project_videos_table"] = """
CREATE TABLE if not EXISTS project_videos (
    project_id integer PRIMARY KEY,
    video_id integer
); """
create_tables["sql_create_project_users_table"] = """
CREATE TABLE if not EXISTS project_users (
    project_id integer PRIMARY KEY,
    user_id integer
); """

# Create a database connection
conn = create_connection(database)
 # create tables

if conn is not None:
    for table in create_tables:
        create_table(conn, create_tables[table])

else:
    print("Error! cannot create the database connection")