cd "$(dirname "$0")"
rm DistributionKit -r
echo start cqtdeplyer | lolcat 
cqtdeployer -bin build-WebUpdatingIndicator-Desktop_Qt_5_12_5_GCC_64bit-Release/WebUpdatingIndicator -qmake ~/Qt/5.12.5/gcc_64/bin/qmake 
echo end cqtdeplyer | lolcat 
cp Logo.ico DistributionKit
cp pages.json DistributionKit
if [ -n "$1" ];
then
	echo "make zip $1" | lolcat
	cp -r DistributionKit $1
	bname=$(basename "$1")
	cd "$(dirname "$1")"
	echo dirname $(dirname "$1") | lolcat
	echo basename $bname | lolcat
	rm $bname.zip
	zip -9 $bname.zip -r $bname/*  
	rm -r $bname
fi

