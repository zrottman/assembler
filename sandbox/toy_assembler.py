dest_table = {
    'M'  :'001',
    'D'  :'010',
    'MD' :'011',
    'A'  :'100',
    'AM' :'101',
    'AD' :'110',
    'AMD':'111'
}

jump_table = {
    'JGT':'001',
    'JEQ':'010',
    'JGE':'011',
    'JLT':'100',
    'JNE':'101',
    'JLE':'110',
    'JMP':'111'
}

comp_table = {
    '0'  :'0101010',
    '1'  :'0111111',
    '-1' :'0111010',
    'D'  :'0001100',
    'A'  :'0110000',
    'M'  :'1110000',
    '!D' :'0001101',
    '!A' :'0110001',
    '!M' :'1110001',
    '-D' :'0001111',
    '-A' :'0110011',
    '-M' :'1110011',
    'D+1':'0011111',
    'A+1':'0110111',
    'M+1':'1110111',
    'D-1':'0001110',
    'A-1':'0110010',
    'M-1':'1110010',
    'D+A':'0000010',
    'D+M':'1000010',
    'D-A':'0010011',
    'D-M':'1010011',
    'A-D':'0000111',
    'M-D':'1000111',
    'D&A':'0000000',
    'D&M':'1000000',
    'D|A':'0010101',
    'D|M':'1010101',
}

dest_key, dest_val = zip(*dest_table.items())
jump_key, jump_val = zip(*jump_table.items())
comp_key, comp_val = zip(*comp_table.items())

dest_key = list(dest_key)
dest_val = [int("0b"+v, 2) for v in dest_val]
jump_key = list(jump_key)
jump_val = [int("0b"+v, 2) for v in jump_val]
comp_key = list(comp_key)
comp_val = [int("0b"+v, 2) for v in comp_val]

def build_C_instruction(comp, dest, jump):
    comp, dest, jump  = lookup_C_instruction(comp, dest, jump)
    c_instruction = 7
    c_instruction <<= 3
    c_instruction |= comp
    c_instruction <<= 7
    c_instruction |= dest
    c_instruction <<= 3
    c_instruction |= jump
    return c_instruction

def tokenize_C_instruction(c_instruction):
    comp = dest = jump = None
    chunk = c_instruction.split('=')
    if len(chunk) == 1:
        dest, jump = chunk[0].split(';')
    else:
        dest, comp = chunk 
        chunk = comp.split(';')
        if len(chunk) == 2:
            jump = chunk[1]

    print("comp instruction: {}".format(comp))
    print("dest instruction: {}".format(dest))
    print("jump instruction: {}".format(jump))

    return comp, dest, jump

def lookup_C_instruction(comp, dest, jump):

    try:
        comp = comp_val[comp_key.index(comp)]
    except:
        comp = 0
    
    try:
        jump = jump_val[jump_key.index(jump)]
    except:
        jump = 0

    try:
        dest = dest_val[dest_key.index(dest)]
    except:
        dest = 0
    
    return comp, dest, jump
        


