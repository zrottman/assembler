#############################
# WRITING AN ASSEMBLER IN C #
#############################

# - initiated at impossible stuff day
# - last time: presented on encoding simple address commands to 16-bit machine 
#   code. @6 => 0000000000000110
# - now: parsing C instructions and handling arbitrary symbols/labels <-- what 
#   I'm going to talk about
#
# Presentation:
#   1. Demo
#   2. C-instruction lookups using parallel arrays
#   3. Symbol/label handling with a linked list

# (disclaimer: using python for legibility)

































###########
# 1. DEMO #
###########

# demo the assembler!







































############################
# 2. C-INSTRUCTION LOOKUPS #
############################

# C-instruction anatomy:
#       <destination>=<computation>;<jump>  => MD=M-1;JMP
#                     <computation>;<jump>  => D;JGT
#       <destination>=<computation>         => M=D+1

# EXAMPLE 1
# MD=M-1;JMP
#   dest: MD  => '011'
#   comp: M-1 => '1110010'
#   jump: JMP => '111'
#
#   c-instr: 111 1110010 011 111

# EXAMPLE 2
# D;JGT 
#   dest:     => '000'
#   comp: D   => '0001100'
#   jump: JMP => '001'
#
#   c-instr: 111 0001100 000 001

# EXAMPLE 3
# M=D+1
#   dest: M   => '001'
#   comp: D+1 => '0011111'
#   jump:     => '000'
#
#   c-instr: 111 0011111 001 000


# Lookups using dictionary/hash table
dest = {
        "M"  : "001",
        "D"  : "010",
        "MD" : "011",
        "A"  : "100",
        "AM" : "101",
        "AD" : "110",
        "AMD": "111"
        }

dest['AM'] # ==> "101"

# Lookups using parallel arrays 
dest_keys = [ "M", "D", "MD", "A", "AM", "AD", "AMD" ]
dest_vals = [ "001", "010", "011", "100", "101", "110", "111" ]

dest_vals[dest_keys.index('AM')] # ==> "101"



# That's what I did in C! (except using integer equivalents)


# dest key/value pairs
dest_keys = [ "M", "D", "MD", "A", "AM", "AD", "AMD" ]
dest_vals = [ 1, 2, 3, 4, 5, 6, 7 ]

# jump key/value pairs
jump_keys = [ "JGT", "JEQ", "JGE", "JLT", "JNE", "JLE", "JMP" ]
jump_vals = [ 1, 2, 3, 4, 5, 6, 7]
#
# comp key/value pairs
comp_keys = [
    "0", "1", "-1", "D", "A", "M", "!D", "!A", "!M", "-D", "-A", "-M",
    "D+1", "A+1", "M+1", "D-1", "A-1", "M-1", "D+A", "D+M", "D-A", "D-M",
    "A-D", "M-D", "D&A", "D&M", "D|A", "D|M"
    ]
comp_vals = [
    42, 63, 58, 12, 48, 112, 13, 49, 113, 15, 51, 115, 31, 55, 119, 14, 
    50, 114, 2, 66, 19, 83, 7, 71, 0, 64, 21, 85
    ]







































#####################
# 3. SYMBOLS/LABELS #
#####################

# works like the other lookups...

symbols = {                                                    
   'SP'    :0,                                                 
   'LCL'   :1,                                                 
   'ARG'   :2,                                                 
   'THIS'  :3,                                                 
   'THAT'  :4,                                                 
   'R0'    :0,                                                 
   'R1'    :1,                                                 
   'R2'    :2,                                                 
   'R3'    :3,                                                 
   'R4'    :4,                                                 
   'R5'    :5,                                                 
   'R6'    :6,                                                 
   'R7'    :7,                                                 
   'R8'    :8,                                                 
   'R9'    :9,                                                 
   'R10'   :10,                                                
   'R11'   :11,                                                
   'R12'   :12,                                                
   'R13'   :13,                                                
   'R14'   :14,                                                
   'R15'   :15,                                                
   'SCREEN':16384,                                             
   'KBD'   :24576                                              
   }                                                           

# ... except we need to be able to add new symbols we find and assign them a unique value


# @SP => 0  <---- key found
# ...
# @i =>  16 <---- key not found, assign it unique number and increment
# ...
# @j =>  17 <---- key not found, assign it unique number and increment
# ...
# @i =>  16 <---- key found


# My solution: use a linked list where each node has a `key` (a string) and a `value` (a unique integer)



# What the linked list will look like in <key>/<value> format
# SP/0 -> LCL/1 -> ARG/2 -> ... -> KBD/24576 -> i/16 -> j/17 -> 



# Show the code !

# C version runs 10x faster for an assembly program consisting of 27,000+ lines 
# in spite of linear lookups. Cool!


# I had a lot of help making this happen. Thanks Miccah, Cormac, and Conrad!

































def lookup(key, command):

    match command:
        case "dest":
            keys, values = dest_keys, dest_vals
        case "jump":
            keys, values = jump_keys, jump_vals
        case "comp":
            keys, values = comp_keys, comp_vals
        
    try:
        val = values[keys.index(key)]
    except ValueError:
        val = 0

    if command == "comp":
        print("`{}` command {:<4} => {:07b}".format(command, key, val))
    else:
        print("`{}` command {:<4} => {:03b}".format(command, key, val))

    return val

if __name__ == '__main__':
    
    lookup("D", "dest")
    lookup("JEQ", "comp")
    lookup("A-D", "comp")
    lookup("JLE", "jump")

