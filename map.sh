#!/bin/bash

i=2
id=${!i}

# integer
re='^[0-9]+$'

FILE=$1

# First arg is not a file or wasn't passed at all
if [ -z $FILE ] || ! [ -f $FILE ]
then
    exit 1
fi

# While there is an argument to be processed
while [ -n "$id" ]
do
	# If id is not valid
	if ! [[ $id =~ $re ]] || [[ $id -lt 1 ]] || [[ $id -gt 999 ]]
	then
		exit 1
	fi

	while read line
	do
		
		currId=$(echo $line| cut -d';' -f 1 )

		# If currId is empty
		if ! [[ $currId =~ $re ]]
		then
   			continue
		fi

		# If route was found
		if [ $currId -eq $id ]
		then

			length=0
			y=3
			tmp=$(echo $line| cut -d';' -f $y )

			# While there is a segment of the route to be processed
			while [ -n "$tmp" ]
			do

				(( length += $tmp ))
				(( y += 3 ))
				tmp=$(echo $line| cut -d';' -f $y )
			done

			echo "$id;$length"

		fi

	done < $FILE

	((i++))
	id=${!i}	
done

# If main while wasn't entered at all
if [ $i -eq 2 ]
then
	exit 1
fi
