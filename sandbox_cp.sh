#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
echo "Syncing Database from @sandbox to local"
drush -y sql-sync @sandbox @self
echo "Synced Database"
echo "Syncing Files"
drush -y rsync @sandbox:%files/ @self:%files
echo "Synced Files"

