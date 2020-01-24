cd "$(dirname "$0")"
rm DistributionKit -r
echo start cqtdeplyer | lolcat 
cqtdeployer -bin build-WebUpdatingIndicator-Desktop_Qt_5_12_5_GCC_64bit-Release/WebUpdatingIndicator -qmake ~/Qt/5.12.5/gcc_64/bin/qmake 
echo end cqtdeplyer | lolcat 
cp Logo.ico DistributionKit

