del /S/Q/F DistributionKit > nul
rmdir /Q/S DistributionKit
bash.exe -c "echo start cqtdeplyer | lolcat"
"C:\Program Files (x86)\CQtDeployer\1.3\cqtdeployer.exe" -bin release\WebUpdatingIndicator.exe -qmake C:\Qt\Qt5.6.2\5.6\mingw49_32\bin\qmake.exe
bash -c "echo end cqtdeplyer | lolcat"
copy Logo.ico DistributionKit"
copy pages.json DistributionKit"
copy C:\Qt\Qt5.6.2\Tools\mingw492_32\opt\bin\libeay32.dll DistributionKit
copy C:\Qt\Qt5.6.2\Tools\mingw492_32\opt\bin\ssleay32.dll DistributionKit
