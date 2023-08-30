# Assembler

An assembler! Converts assembler language to 16-bit machine code.

## Hack Spec

This program assembles Hack, a simple assembly language from *The Elements of Computing Systems* (aka *Nand to Tetris*). Hack comprises of three kinds of instructions:
- **A(ddress) instructions** begin with `@` and load a 15-bit memory address into the CPU's A register
- **C(ompute) instructions** begin with either a destination or computation code and tell the CPU which registers to read from, what to do with those values, where to store the result, and whether to jump elsewhere in the code.
- **L(abel) instructions** are enclosed in parentheses. These are pseudocommands since they are used to label line numbers to enable jumping, but they are not themselves converted into assembly.

<style type="text/css">
  table, tr, td {
    border: none;
    text-align: center;
  }

  .array td {
    border: 1px solid black;
    /*background-color: white;*/
    width: 30px;
    height: 30px;
  }

  .bitsPlace { color: gray; }

  .i { background: #d3f6db; }
  .a { background: #92d5e6; }
  .c { background: #772d8b; color: #ccc; }
  .d { background: #5a0b4d; color: #ccc; }
  .j { background: #a1ef8b; }

</style>

### A Instructions

**A(ddress) instructions** are the simplest, since converting them to 16-bit machine code instructions is a matter of converting the number to a 15-bit integer and appending a `0` on the front, which is what identifies the instruction as an A Instruction.

<table>
  <tr class='array'>
    <td class="i">0</td>
    <td>v</td>
    <td>v</td>
    <td>v</td>
    <td>v</td>
    <td>v</td>
    <td>v</td>
    <td>v</td>
    <td>v</td>
    <td>v</td>
    <td>v</td>
    <td>v</td>
    <td>v</td>
    <td>v</td>
    <td>v</td>
    <td>v</td>
  </tr>
  <tr class='bitsPlace'>
    <td>15</td>
    <td>14</td>
    <td>13</td>
    <td>12</td>
    <td>11</td>
    <td>10</td>
    <td>9</td>
    <td>8</td>
    <td>7</td>
    <td>6</td>
    <td>5</td>
    <td>4</td>
    <td>3</td>
    <td>2</td>
    <td>1</td>
    <td>0</td>
  </tr>
</table>

- <span class="i">the most significant bit</span> (set to 0) identifies the instruction as an A instruction
- the least significant 15 bits represent a 15-bit unsigned integer

### C Instructions

**C(omputation) Instructions** have three component parts that are encoded into the 16-bit instruction string. For sample C-instructions like "AD=M+1" or "D;JEQ" or "D=1" or "D=M-1;JLT":
- everything to the left of the `=` is the destination part of the command -- i.e., where the ALU's output needs to go. (**D** is the destination in the command `D=M+1`.) This piece of information gets encoded as a 3-bit sequence and spans bits 3-5 of the resulting instruction string.
- everything to the right of the `=` and to the left of the `;` is the computation command -- i.e., what computation the ALU performs on its inputs. (**M+1** is the computation in the command `D=M+1`.) There are a lot of options here, so this piece of information gets encoded as a 7-bit sequence and spans bits 6-12 of the resulting instruction string.
- everything right of the `;` is the jump command, which, if present, loads the value in the A Register (in this case representing a program line number) into the Program Counter, thus "jumping" to another line in the program. (**JGT** specifies the jump in the command `D;JGT`.) This piece of information gets encoded as a 3-bit sequence and spans bits 0-2 in the resulting instruction string.

<table>
  <tr class='array'>
    <td class="i">1</td>
    <td>x</td>
    <td>x</td>
    <td class="a">a</td>
    <td class="c">c</td>
    <td class="c">c</td>
    <td class="c">c</td>
    <td class="c">c</td>
    <td class="c">c</td>
    <td class="c">c</td>
    <td class="d">d</td>
    <td class="d">d</td>
    <td class="d">d</td>
    <td class="j">j</td>
    <td class="j">j</td>
    <td class="j">j</td>
  </tr>
  <tr class='bitsPlace'>
    <td>15</td>
    <td>14</td>
    <td>13</td>
    <td>12</td>
    <td>11</td>
    <td>10</td>
    <td>9</td>
    <td>8</td>
    <td>7</td>
    <td>6</td>
    <td>5</td>
    <td>4</td>
    <td>3</td>
    <td>2</td>
    <td>1</td>
    <td>0</td>
  </tr>
</table>

- <span class="i">the most significant bit</span> (set to 1) identifies the instruction as a C Instruction
- bits 13 and 14 aren't used
- <span class="a">the <b>a</b> bit</span> (bit 12) dictates whether the ALU should accept its `y` input from the A Register or from somewhere in memory. This is effectively part of the computation instruction.
- <span class="c">the 6 <b>c</b> bits</span> (bits 6-11) specify the ALU function and are fed into its 6 function inputs `zx`, `nx`, `zy`, `ny`, `f`, and `no`.
- <span class="d">the 3 <b>d</b> bits</span> (bits 3-5) specify the destination for the ALU's output, either `M` (memory), `A` (A register), or `D` (D register), or some combination of the three.
- <span class="j">the 3 <b>j</b> bits</span> (bits 0-2) specify any jump command based on whether the ALU's output is `< 0`, `<= 0`, `== 0`, `>= 0`, or `> 0`.

The binary encodings for each of these component parts are as follows:

**Computation  (bits 6-12)**

<table style="border-collapse: collapse;">
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0; border-bottom: 2px solid black;">zx</th>
    <th style="padding: 10px; text-align: center; border-right: 0; border-bottom: 2px solid black;">nx</th>
    <th style="padding: 10px; text-align: center; border-right: 0; border-bottom: 2px solid black;">zy</th>
    <th style="padding: 10px; text-align: center; border-right: 0; border-bottom: 2px solid black;">ny</th>
    <th style="padding: 10px; text-align: center; border-right: 0; border-bottom: 2px solid black;">f</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black; border-bottom: 2px solid black;">no</th>
    <th style="padding: 10px; text-align: center; border-right: 0; border-bottom: 2px solid black;">out</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">-1</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">x</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">y</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">!x</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">!y</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">-x</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">-y</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">x+1</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">y+1</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">x-1</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">y-1</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">x+y</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">x-y</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">y-x</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">x&y</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">x|y</th>
  </tr>
</table>

**Destination (bits 4-6)**

<table style="border-collapse: collapse;">
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0; border-bottom: 2px solid black;">d<sub>5</sub></th>
    <th style="padding: 10px; text-align: center; border-right: 0; border-bottom: 2px solid black;">d<sub>4</sub></th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black; border-bottom: 2px solid black;">d<sub>3</sub></th>
    <th style="padding: 10px; text-align: center; border-right: 0; border-bottom: 2px solid black;">Mnemonic</th>
    <th style="padding: 10px; text-align: center; border-right: 0; border-bottom: 2px solid black;">Destination</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">null</th>
    <th style="padding: 10px; text-align: left; border-right: 0;">Don't store anywhere</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">M</th>
    <th style="padding: 10px; text-align: left; border-right: 0;">Memory[A]</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">D</th>
    <th style="padding: 10px; text-align: left; border-right: 0;">D Register</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">MD</th>
    <th style="padding: 10px; text-align: left; border-right: 0;">Memory[A], D Register</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">A</th>
    <th style="padding: 10px; text-align: left; border-right: 0;">A Register</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">AM</th>
    <th style="padding: 10px; text-align: left; border-right: 0;">A Register, Memory[A]</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">AD</th>
    <th style="padding: 10px; text-align: left; border-right: 0;">A Register, D Register</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">AMD</th>
    <th style="padding: 10px; text-align: left; border-right: 0;">A Register, Memory[A], D Register</th>
  </tr>
</table>

**Jump (bits 1-3)**

<table style="border-collapse: collapse;">
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0; border-bottom: 2px solid black;">j<sub>2</sub></th>
    <th style="padding: 10px; text-align: center; border-right: 0; border-bottom: 2px solid black;">j<sub>1</sub></th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black; border-bottom: 2px solid black;">j<sub>0</sub></th>
    <th style="padding: 10px; text-align: center; border-right: 0; border-bottom: 2px solid black;">Mnemonic</th>
    <th style="padding: 10px; text-align: center; border-right: 0; border-bottom: 2px solid black;">Jump Command</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">null</th>
    <th style="padding: 10px; text-align: left; border-right: 0;">No jump</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">JGT</th>
    <th style="padding: 10px; text-align: left; border-right: 0;">Jump if <i>out</i> > 0</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">JEQ</th>
    <th style="padding: 10px; text-align: left; border-right: 0;">Jump if <i>out</i> = 0</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">JGE</th>
    <th style="padding: 10px; text-align: left; border-right: 0;">Jump if <i>out</i> >= 0</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">JLT</th>
    <th style="padding: 10px; text-align: left; border-right: 0;">Jump if <i>out</i> < 0</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">JNE</th>
    <th style="padding: 10px; text-align: left; border-right: 0;">Jump if <i>out</i> != 0</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">0</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">JLE</th>
    <th style="padding: 10px; text-align: left; border-right: 0;">Jump if <i>out</i> <= 0</th>
  </tr>
  <tr>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 2px solid black;">1</th>
    <th style="padding: 10px; text-align: center; border-right: 0;">JMP</th>
    <th style="padding: 10px; text-align: left; border-right: 0;">Jump</th>
  </tr>
</table>

### L Instructions

**L(abel) Instructions** are pseudo-instructions, since they do not directly translate into equivalent lines of machine code. Instead, when L Instructions are encoundered, they are added to a symbols table as references to corresponding line numbers.

## Two Pass Approach

This assembler performs two passes on an input .asm file. 

The first pass identifies L Instructions and adds them to a symbols table with a corresponding line number, so that jump instructions specifying that label know to which line of machine code to jump.

The second pass handles A and C instructions. When symbols are encountered, these are added to the symbols table with an arbitrarily assigned memory address such that each symbol corresponds to a unique address.

## Usage

### 1. Compilation:

Before using the assembler, you need to compile it. Open a terminal window and navigate to the directory containing the assembler's source code. Then, use your preferred C compiler to build the executable. For example, if you're using CC, you can use the following command:

```bash
cc assembler.c linkedlist.c -o assembler
```

This will create an executable file named "assembler" in the same directory.

### 2. Assembling:

Once the assembler has been compiled, you can use it to translate your assembly code into machine code. The assembler takes one command line argument, which is the path to the assembly file you want to translate. The command follows this format:

```bash
./assembler path/to/your/assembly_file.asm
```

Replace path/to/your/assembly_file.asm with the actual path to your assembly file.

### 3. Output:

After running the above command, the assembler will process the assembly file and generate the corresponding machine code. The output will be saved to a new file with the same name as the input assembly file, but with a different extension (e.g., .hack). You can find this output file in the same directory where your assembly file is located.

## Example

Here's an example of how you could use the assembler to translate an assembly file named "example.asm":

```bash
./assembler example.asm
```

After running this command, you will find the assembled machine code in a file named "example.hack" in the same directory.

```
// sample input
@R0
D=M
@R1
D=D-M
@OUTPUT_FIRST
D;JGT
@R1
D=M
@OUTPUT_D
0;JMP
(OUTPUT_FIRST)
@R0
D=M
(OUTPUT_D)
@R2
M=D
(INFINITE_LOOP)
@INFINITE_LOOP
0;JMP
```

```
// sample output
0000000000000000
1111110000010000
0000000000000001
1111010011010000
0000000000001010
1110001100000001
0000000000000001
1111110000010000
0000000000001100
1110101010000111
0000000000000000
1111110000010000
0000000000000010
1110001100001000
0000000000001110
1110101010000111
```

## Misc.

This assembler is a C port of the Python assembler I built during my Summer 2023 batch at the Recurse Center while completing [The Elements of Computing Systems](https://github.com/zrottman/nand2tetris).
