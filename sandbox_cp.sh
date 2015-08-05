#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

if [[ $# -eq 0 ]] ; then
    echo "✘ Try again with your username as first argument"
    exit 0
fi

echo "Syncing Database from @sandbox to local..."
drush -y @sandbox sql-dump --result-file=/tmp/cdpp.sql && rsync $1@sandbox:/tmp/cdpp.sql /tmp/cdpp.sql && drush -y sql-drop && drush -y sql-cli < /tmp/cdpp.sql && echo "✔ Synced Database" || echo "✘ An Error Occured."
echo ""

echo "Syncing Files..."
drush -y rsync @sandbox:%files/ @self:sites/default/files && echo "✔ Synced Files" || echo "✘ An Error Occured."
echo ""

echo "Clearing Caches..."
drush cc all && echo "✔ Cached Cleared" || echo "✘ An Error Occured."
echo ""

echo "Repopulating Cache..."
echo "- Homepage..."
wget --timeout=1 -qO- http://cdpp.dev/ &> /dev/null
wget --timeout=1 -qO- http://cdpp.local/ &> /dev/null
echo "- Login Page..."
wget --timeout=1 -qO- http://cdpp.dev/user &> /dev/null
wget --timeout=1 -qO- http://cdpp.local/user &> /dev/null
# Just say it because either .local or .dev will always fail...
echo "✔ Cached Regenerated"
echo ""

echo "Checking for theme updates..."
# http://stackoverflow.com/questions/3258243/git-check-if-pull-needed
[ $(git rev-parse HEAD) = $(git ls-remote $(git rev-parse --abbrev-ref @{u} | \
sed 's/\// /g') | cut -f1) ] && echo "✔ Up To Date" || echo "✘ Updates Available"
echo ""

echo "☺ All done."
echo ""
