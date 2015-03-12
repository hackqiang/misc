#!/bin/bash
nn=`cat t1 | grep -n "<td class=\"name\">品牌</td>" | awk '{print $1}'| sed -e 's/:/ /'`
let "nn=$nn+1"
echo $nn
model_tmp=`head -n $nn t1 | tail -n 1`
echo $model_tmp
model=`echo -n $model_tmp | sed -e 's/class=\"desc\">/ /' | awk '{print $2}' | sed -e 's/<\/td>/\ /' | awk '{print $1}'`	
echo -e -n $model $model $model	