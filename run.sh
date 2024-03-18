#!/bin/bash

DB_USER="root" 
DB_PASS="Test_password1." ## Not my actual password for anything but i recognise this as a potential security issue
DB_NAME="userDatabase"
BACKUP_PATH="./db/userDatabase_backup.sql"

echo "Restoring database from backup..."
mysql -u $DB_USER -p$DB_PASS $DB_NAME < $BACKUP_PATH

if [ $? -ne 0 ]; then
    echo "Failed to restore database from backup."
    exit 1
fi

echo "Compiling C++ program..."
g++ -Wall src/main.cpp -o main -lmysqlcppconn

if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

echo "Running application..."
./main

echo "Creating database backup..."
mysqldump -u $DB_USER -p$DB_PASS $DB_NAME > $BACKUP_PATH

if [ $? -ne 0 ]; then
    echo "Failed to create database backup."
    exit 1
fi

echo "Operation completed successfully."
