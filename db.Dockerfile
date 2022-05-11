FROM postgres
ENV POSTGRES_DB cavoke
COPY ./server/db/schema.sql /docker-entrypoint-initdb.d/