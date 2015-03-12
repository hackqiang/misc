#!/bin/bash

url1="http://searchex.yixun.com/html?path=705882t705892&attr=904e9&area=1&sort=0&show=0&page=1&size=40&pf=0&as=0&charset=utf-8#list"
url2="http://searchex.yixun.com/html?path=705882t705892&attr=904e9&area=1&sort=0&show=0&page=1&size=40&pf=0&as=0&charset=utf-8#list"
url3="http://searchex.yixun.com/html?path=705882t705892&attr=904e9&area=1&sort=0&show=0&page=1&size=40&pf=0&as=0&charset=utf-8#list"

if [ 1 -eq 0 ]; then

rm urls.txt

wget $url1 -O p1
wget $url2 -O p2
wget $url3 -O p3

cat p2 >>p1
cat p3 >>p1



cat p1 | while read line
do
	echo  $line | grep -q "<a href=\"http://item.yixun.com/item-"
	if [ $? -eq 0 ]; then
		echo $line >>urls.txt
	fi
done

fi

rm size.txt
rm tmp
lineno=1
cat urls.txt | while read line
do
	#get URL
	url=`echo $line | sed -e 's/href="/ /' | awk '{print $3}'| sed -e 's/\"/ /'`
	wget $url -O tmp -o tmp.log
	
	#get SIZE
	nn=`cat tmp | grep -n "<td class=\"name\">尺寸</td>" | awk '{print $1}'| sed -e 's/:/ /'`
	let "nn=$nn+1"
	sizes_tmp=`head -n $nn tmp | tail -n 1`
	
	size=`echo -n $sizes_tmp | sed -e 's/class=\"desc\">/ /' | awk '{print $2}' | sed -e 's/<\/td>/\ /' | awk '{print $1}'`
	
	#get model
	nn=`cat tmp | grep -n "<td class=\"name\">品牌</td>" | awk '{print $1}'| sed -e 's/:/ /'`
	let "nn=$nn+1"
	model_tmp=`head -n $nn tmp | tail -n 1`
	model=`echo -n $model_tmp | sed -e 's/class=\"desc\">/ /' | awk '{print $2}' | sed -e 's/<\/td>/\ /' | awk '{print $1}'`	
	
	echo -e "$lineno\t$url"
	echo -e "$lineno\t\t\t$model\t\t\t$size\t\t\t$url" >> size.txt
	let lineno=lineno+1
done
