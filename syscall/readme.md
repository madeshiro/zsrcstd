# Zsr C Std Library - Syscall #
> *ZaeSora **C** Standard Library*, **asm** `api`   
> **zsrsyscall.(a|so)**
* * *
*ZaeSora development | **ZsrLibraries** series*

## Description
## Syscall tables `64bit`
```asm
# file <z64sys.S>
.att_syntax
.macro zsyscall
    call z64syscall
.endm

# //// #

zsyscall
```
> Include <z64sys.S> to get macro `zsyscall` and all functions' code

<table>
    <thead>
        <th>Name</th>
        <th>rax</th>
        <th>rdi</th>
        <th>rsi</th>
        <th>rdx</th>
        <th>r8</th>
        <th>r9</th>
        <th>r10</th>
        <th>@return rax</th>
        <th>Description</th>
    </thead>
    <tbody style="text-align: center;">
        <tr>
            <td>read</td>
            <td><code>0x0</code></td>
            <td><code>zhandler</code><br/>handler input</td>
            <td><code>char*</code><br/>buf</td>
            <td><code>uint32</code><br/>count</td>
            <td colspan="3"></td>
            <td><code>int32</code><br/>Count of read's chars</td>
            <td>Open a file with the specified openm</td>
        </tr>
        <tr>
            <td>write</td>
            <td><code>0x1</code></td>
            <td><code>zhandler</code><br/>handler output</td>
            <td><code>const char*</code><br/>buf</td>
            <td><code>uint32</code><br/>count</td>
            <td colspan="3"></td>
            <td><code>int32</code><br/>number of write's chars</td>
            <td>Close any supported zhandler</td>
        </tr>
        <tr>
            <td>fopen</td>
            <td><code>0x2</code></td>
            <td><code>const char*</code><br/>file's name (relative or absolute)</td>
            <td><code>uint32</code><br/>openm</td>
            <td colspan="4"></td>
            <td><code>zhandler</code><br/>A file's handler (or NULL on error)</td>
            <td>Open a file with the specified openm</td>
        </tr>
        <tr>
            <td>close</td>
            <td><code>0x3</code></td>
            <td><code>zhandler</code><br/>a handler to close</td>
            <td colspan="5"></td>
            <td><code>bool</code><br/>close status (1: succes, 0: error (check errno(<code>0x...</code>))</td>
            <td>Close any supported zhandler</td>
        </tr>
        <tr>
            <td colspan="10"><i>Work in progress...</i></td>
        </tr>
    </tbody>
</table>

### C Programming : how work functions' code ?
