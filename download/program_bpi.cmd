setMode -bs
setCable -p auto
identify
identifyMPM 
attachflash -position 2 -bpi "28F256P30"
assignfiletoattachedflash -position 2 -file "system.mcs"
Program -p 2 -dataWidth 16 -rs1 NONE -rs0 NONE -bpionly -e -v -loadfpga 
closeCable
quit
