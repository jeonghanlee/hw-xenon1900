#!/bin/bash
#
# Author : Jeong Han Lee

this_script_name=`basename $0`
LOGDATE=`date +%Y.%m.%d.%H:%M`
host_name=${HOSTNAME}


goApps() {

     if [ -z ${EPICS_APPS} ]; then
        echo "No EPICS environment variables are found. Force to set the default one."
        echo "No EPICS environment variables are found. Force to set the default one."
        echo ". ~/epics_env/setEpicsEnv.bash"
        echo ""
        . ~/epics_env/setEpicsEnv.bash
     fi
     if [[ $# -eq 0 ]] ; then
        cd ${EPICS_APPS}
     else
        cd ${EPICS_APPS}
        cd $1
     fi
}


goIoc() {
    goApps $1
    cd iocBoot/ioc$1
}

ioc_name="$1"
 
case "$ioc_name" in
    "")
	echo "">&2
        echo "usage: $0 <IOC name>" >&2
        echo >&2
        echo "  IOC name : example " >&2
	echo ""
        echo "       $0 keithley6514 ">&2
        echo "       $0 snmp2 ">&2
        echo ""
        echo >&2
	exit 0
        ;;
    *)
	CHECKPLAT=$(eval screen -ls |grep ${ioc_name})
	if [ -z "$CHECKPLAT" ] ; then
	    
	    case `uname -sm` in
		"Linux i386" | "Linux i486" | "Linux i586" | "Linux i686")
		    EPICS_HOST_ARCH=linux-x86
		    ;;
		"Linux x86_64")
		    EPICS_HOST_ARCH=linux-x86_64
		    ;;
		"Linux armv6l")
		    EPICS_HOST_ARCH=linux-arm
		    ;;
		"Linux armv7l")
		    EPICS_HOST_ARCH=linux-arm
		    ;;
		*)
		    echo "This script  doesn't support this architecture : `uname -sm`"
		    exit 1
		    ;;
	    esac
	    
	    goIoc ${ioc_name}
	    screen -fn -S ${ioc_name} -c ${EPICS_APPS}/.ioc_screenrc -t ${ioc_name} ${EPICS_APPS}/${ioc_name}/bin/${EPICS_HOST_ARCH}/${ioc_name} st.cmd.sd
	    
	else
	    echo "${ioc_name} IOC is running o $HOSTNAME, attaching ...."
	    screen -x $ioc_name
	fi

	;;

esac




exit 0
