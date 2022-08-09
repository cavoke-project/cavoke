FROM postgres
ENV POSTGRES_DB cavoke
ENV POSTGRES_USER postgres_user
ENV POSTGRES_PASSWORD postgres_password
COPY ./server/db/schema.sql /docker-entrypoint-initdb.d/