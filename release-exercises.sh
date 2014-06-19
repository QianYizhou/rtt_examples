#!/bin/bash

if [ x$2 = x ] ; then 
echo "Please provide version-string parameter (e.g. 2.0.1) and release (e.g. 2.0)"
exit 1
fi;

if [ ! -d export ] ; then 
mkdir export || exit 1
fi;

VERSION=$1 
RELEASE=$2

#Make sure we start from right branch.
git checkout rtt-$RELEASE-examples || exit 1

#Merge latest changes into solution branch
git checkout rtt-$RELEASE-solution || exit 1
git rebase rtt-$RELEASE-examples || exit 1

#cleanup
rm -rf export/rtt-exercises-$VERSION
rm -f rtt-exercises-$VERSION.tar.gz 
#copy over and make sure it's clean.
git archive --format=tar --prefix=orocos-rtt-examples-$VERSION/ HEAD | (cd export && tar xf -)

cd export
cp -a orocos-rtt-examples-$VERSION/rtt-exercises rtt-exercises-$VERSION

cd rtt-exercises-$VERSION
#create solution dir.
#hacky, need to improve this
for dirname in controller-1 hello-1-task-execution hello-2-properties hello-3-dataports hello-4-operations hello-6-scripting; do
cp -a $dirname $dirname-solution
done

# now remove solution from original dirs:
git diff rtt-$RELEASE-examples..rtt-$RELEASE-solution | patch -p2 -R || exit 1
#rename Eclipse project files.
for dirname in controller-1 hello-1-task-execution hello-2-properties hello-3-dataports hello-4-operations hello-6-scripting; do
cd $dirname-solution
sed -i -e "s/$dirname/$dirname-solution/g" .project .cproject || exit 1
cd ..
done

cd ..

tar -cvzf rtt-exercises-$VERSION.tar.gz rtt-exercises-$VERSION
cd ..
echo "Packaging done."

echo "Press a key to copy files to server, Ctrl-C to abort..."
read -s -n1

  USER=psoetens
  SERVER=ftp.mech.kuleuven.be
  SPREFIX=orocos/pub
  BRANCH=stable

# Orocos Examples
ssh -l$USER $SERVER "mkdir -p $SPREFIX/$BRANCH/examples/rtt/tutorial" || exit 1
scp export/rtt-exercises-$VERSION.tar.gz $USER@$SERVER:$SPREFIX/$BRANCH/examples/rtt/tutorial || exit 1
scp export/rtt-exercises-$VERSION/README $USER@$SERVER:$SPREFIX/$BRANCH/examples/rtt/tutorial/README.txt || exit 1

echo "Copied files to $SERVER. Done!"
