#!/bin/bash

# Get the directory name of the current script
SCRIPT_DIR=$(pwd)

# Navigate to the parent directory recursively until we find the root directory
ROOT_DIR=""
while [[ "$SCRIPT_DIR" != "/" ]]; do
  if [[ -d "$SCRIPT_DIR/peekaboo" ]]; then
    ROOT_DIR="$SCRIPT_DIR/peekaboo"
    break
  fi
  SCRIPT_DIR="$(dirname "$SCRIPT_DIR")"
done

# Check if we found the root directory
if [[ -z "$ROOT_DIR" ]]; then
  echo "Could not find the root directory."
  exit 1
else
  echo "Root directory is: $ROOT_DIR"
fi

cd $ROOT_DIR

# Build Peekaboo
echo "Building Peekaboo..."

make -C server server
make -C client client

echo "=============================="

# Create database
echo "Creating database..."
rm -f bin/profiles.db

# Create a new SQLite3 database file named "profiles.db"
sqlite3 bin/profiles.db <<EOF

-- Create a table named "profiles"
CREATE TABLE IF NOT EXISTS profiles (
  email VARCHAR(100) PRIMARY KEY,
  name VARCHAR(65),
  last_name VARCHAR(65),
  city VARCHAR(100),
  course VARCHAR(100),
  year_of_degree INTEGER,
  skills VARCHAR(300),
  image INTEGER DEFAULT NULL
);

-- Import data from the CSV file "profiles.csv"
.mode csv
.import scripts/profiles.csv profiles

EOF

echo "=============================="

# Create admin password
echo "Para usar o Peekaboo como administrador, primeiro crie uma senha para o usuário administrador"
gcc scripts/create_admin_password.c lib/sha1-c/sha1.c -o create_admin_password
./create_admin_password
rm create_admin_password
echo "=============================="

echo "Peekaboo is ready to use!"