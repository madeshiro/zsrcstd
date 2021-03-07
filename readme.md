# Zsr C Std Library #
> *ZaeSora **C** Standard Library*, for **Zsr's Projects**
* * *
*ZaeSora development | **ZsrLibraries** series*   

## Description
## Features
### generic features
### features handler-necessary

<table>
    <thead>
        <th>Features</th>
        <th>Handler's name</th>
        <th>Description</th>
    </thead>
    <tbody style="text-align: center;">
        <tr>
            <td>String</td>
            <td>zsr_string</td>
            <td></td>
        </tr>
        <tr>
            <td>List</td>
            <td>zsr_list</td>
            <td></td>
        </tr>
        <tr>
            <td>Chain List</td>
            <td>zsr_chainlist</td>
            <td></td>
        </tr>
    </tbody>
</table>

### operating-system features

<table>
    <thead>
        <tr>
            <th><b>Features</b></th>
            <th>Description</th>
            <th colspan="3">Available on</th>
        </tr>
    </thead>
    <tbody style="text-align: center">
        <tr>
            <td rowspan="2" class="features"><b>Console I/O</b></td>
            <td rowspan="2"></td>
            <td style="text-align: center;" rowspan="1" colspan="2">Unix-like system</td>
            <td style="text-align: center;" rowspan="1">Windows OS</td>
        </tr>
        <tr>
            <td rowspan="3">Ubuntu <b>64bit</b></td>
            <td rowspan="3">Debian <b>64bit</b></td>
            <td rowspan="3">Windows 10 <b>64bit</b></td>
        </tr>
        <tr>
            <td rowspan="1" class="features"><b>File I/O</b></td>
            <td rowspan="1"></td>
        </tr>
        <tr>
            <td rowspan="1" class="features"><b>Pipe I/O</b></td>
            <td rowspan="1"></td>
    </tbody>
</table>

***
## Network programming
### Endianness
***A binary mechanical on computer science***  
`how it work`
***
> it's about how binary numbers are stocked into the computer's memory :
> - Big Endian (*Network system*) : high bit first
> - Little Endian (*most commun machine storage system*) : low bit first

#### Example :
Consider the following variables :
```c
char c;     // register_eq: %cl
int i;      // register_eq: %edx
short s;    // register_eq: %bx
```
Consider the following statement :
```c
s = 0x101b; // normal; ENDIANNESS representation 
```
##### BIG ENDIAN

<table>
    <thead>
        <tr>
            <th>BE</th>
            <th colspan="4"><b>64 bits</b></th>
            <th colspan="2"><b>32 bits</b></th>
            <th colspan="2"><b>16 bits</b></th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td rowspan="2" style="text-align: right;"><b>register :</b></td>
            <td colspan="4" rowspan="2" style="text-align: center;">RAX</td>
            <td colspan="2" rowspan="2" style="text-align: center">EAX</td>
            <td colspan="2" style="text-align: center">AX</td>
        </tr>
        <tr>
            <td style="text-align: center;">AH</td>
            <td style="text-align: center;">AL</td>
        </tr>
        <tr>
            <td style="text-align: right;">0x</td>
            <td style="text-align: center;">00</td>
            <td style="text-align: center;">00</td>
            <td style="text-align: center;">00</td>
            <td style="text-align: center;">00</td>
            <td style="text-align: center;">00</td>
            <td style="text-align: center;">00</td>
            <td style="text-align: center;">10</td>
            <td style="text-align: center;">1b</td>
        </tr>
    </tbody>
</table>

##### LITTLE ENDIAN

<table>
    <thead>
        <tr>
            <th>LE</th>
            <th colspan="2"><b>16 bits</b></th>
            <th colspan="2"><b>32 bits</b></th>
            <th colspan="4"><b>64 bits</b></th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td style="text-align: right;" rowspan="2"><b>register :</b></td>
            <td colspan="2" style="text-align: center">AX</td>
            <td colspan="2" rowspan="2" style="text-align: center">EAX</td>
            <td colspan="4" rowspan="2" style="text-align: center;">RAX</td>
        </tr>
        <tr>
            <td style="text-align: center;">AL</td>
            <td style="text-align: center;">AH</td>
        </tr>
        <tr>
            <td style="text-align: right;">0x</td>
            <td style="text-align: center;">1b</td>
            <td style="text-align: center;">10</td>
            <td style="text-align: center;">00</td>
            <td style="text-align: center;">00</td>
            <td style="text-align: center;">00</td>
            <td style="text-align: center;">00</td>
            <td style="text-align: center;">00</td>
            <td style="text-align: center;">00</td>
        </tr>
    </tbody>
</table>

```c
char *bits = &s;
if (is_little_endian())
{
    bits[0] == 0x1b; // instead of 0x10 in big endian
    bits[1] == 0x10; // instead of 0x1b in big endian 
}
```
cause of `ASM translation` :
```asm
mov %bx, -0x8(%rbp)
lea -0x8(%rbp), %rax # char* bits = &s | 0x [1b][10]
mov 0x0(%rax), %cl   # bits[0] : [1b] -> %cl
```
