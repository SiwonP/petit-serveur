-- pg_server--1.0.sql

-- Create the extension schema
CREATE SCHEMA http_server;

CREATE TABLE IF NOT EXISTS http_server.server (
    id serial primary key,
    name varchar not null,
    port integer not null,
    running integer
);

CREATE TABLE IF NOT EXISTS http_server.path (
    id serial primary key,
    id_api int,
    path varchar,
    command varchar
);

-- Create the SQL function
-- CREATE OR REPLACE FUNCTION http_server.factorial(integer)
-- CREATE OR REPLACE FUNCTION factorial(integer)
-- RETURNS bigint AS 'pg_server', 'factorial'
-- LANGUAGE C STRICT;


CREATE OR REPLACE FUNCTION http_server.create_server(name varchar(255), port integer)
RETURNS VOID
AS 'INSERT INTO http_server.server(name, port) values($1, $2);'
LANGUAGE SQL
RETURNS NULL ON NULL INPUT;


CREATE OR REPLACE FUNCTION http_server.list_server()
RETURNS TABLE(id integer, name varchar, port integer, pid integer)
AS 'select * from http_server.server;'
LANGUAGE SQL;


CREATE OR REPLACE FUNCTION http_server.call(path varchar, port integer)
RETURNS TEXT
AS $$
DECLARE 
    cmd text;
BEGIN
    SELECT p.command into cmd from http_server.path p join http_server.api a on p.id_api = a.id where a.port = $2 and p.path = $1;
    RETURN (EXECUTE '$1' USING cmd);
END 
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION http_server.start()
RETURNS int AS 'pg_server', 'start'
LANGUAGE C STRICT;

-- Grant execute permission to public (change to appropriate roles if needed)
-- GRANT EXECUTE ON FUNCTION http_server.factorial(integer) TO PUBLIC;
GRANT EXECUTE ON FUNCTION http_server.create_server(varchar(255), integer) TO PUBLIC;
GRANT EXECUTE ON FUNCTION http_server.list_server() TO PUBLIC;
-- GRANT EXECUTE ON FUNCTION http_server.call() TO PUBLIC;
