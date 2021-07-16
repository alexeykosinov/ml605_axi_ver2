xload xmp system.xmp
run netlist
run bits

if 0 {
	proc pnsynth {} {

		puts "# Loading xmp file"
		if { [ catch { xload xmp system.xmp } result ] } {
			puts "# Error: Loading xmp file"
			exit 10
		}
		
		puts "# Running netlist"
		if { [catch {run netlist} result] } {
			puts "# Error: netlist generation"
			return -1
		}
		
		puts "# Running bitstream generation"
		if { [catch {run bits} result] } {
			puts "# Error: bitstream generation"
			return -1
		}  

		return $result
	}

	if { [catch {pnsynth} result] } {
		exit -1
	}

	exit $result
}