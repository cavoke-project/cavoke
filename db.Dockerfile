FROM postgres
ENV POSTGRES_DB postgres_db
COPY ./server/db/schema.sql /docker-entrypoint-initdb.d/