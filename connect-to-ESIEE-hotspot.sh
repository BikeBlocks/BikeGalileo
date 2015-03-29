	if [[ ! "$1" == "" ]] ; then
		if [[ "$1" == "-l" ]] ; then
			curl 'https://controller.mobile.lan/portal_degraded.php' --cookie-jar /tmp/ucopia > /dev/null
			if [ $(curl 'https://controller.mobile.lan/portal_degraded.php' --referer 'https://controller.mobile.lan/portal_degraded.php' --cookie /tmp/ucopia --data 'action=authenticate&secure_pwd=&login=guest&password=guest&valid=' | grep -q maximum) ] ; then
				echo "Guest account failed"
			else
				echo "Logged as guest"
			fi
		else
			echo "Usage : $0 [-l]"
			return 1
		fi
	fi
