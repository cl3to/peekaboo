#!/bin/bash

# Build Peekaboo
echo "Building Peekaboo..."
cd server
make server
cd ../client
make client
cd ..
echo "=============================="

# Create database
echo "Creating database..."
# TODO - create database
echo "=============================="

# Create admin password
echo "To use Peekaboo as Admin, Fist create a password for the admin user"
gcc scripts/create_admin_password.c lib/sha1-c/sha1.c -o create_admin_password
./create_admin_password
rm create_admin_password
echo "=============================="

echo "Peekaboo is ready to use!"