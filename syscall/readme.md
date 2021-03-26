# Zsr C Std Library - Syscall #
> *ZaeSora **C** Standard Library*, **asm** `api`   
> **zsrsyscall.a** (*static lib*)
* * *
*ZaeSora development | **ZsrLibraries** series*

## Description
## Syscall tables `64bit`
`API VERSION : 0.4.1 (unstable) [sync w/ zsrcstd for now]`
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
        <th>rdi<br/><code>%xmm0</code></th>
        <th>rsi<br/><code>%xmm1</code></th>
        <th>rdx<br/><code>%xmm2</code></th>
        <th>r8<br/><code>%xmm3</code></th>
        <th>r9<br/><code>%xmm4</code></th>
        <th>r10<br/><code>%xmm5</code></th>
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
            <td>Read data from any supported I/O handlers</td>
        </tr>
        <tr>
            <td>write</td>
            <td><code>0x1</code></td>
            <td><code>zhandler</code><br/>handler output</td>
            <td><code>const char*</code><br/>buf</td>
            <td><code>uint32</code><br/>count</td>
            <td colspan="3"></td>
            <td><code>int32</code><br/>number of write's chars</td>
            <td>Write buffers to any supported I/O handler</td>
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
            <td><code>bool</code><br/>close status (1: succes, 0: error (check errno(<code>0x5</code>))</td>
            <td>Close any supported zhandler</td>
        </tr>
        <tr>
            <td>stdio</td>
            <td><code>0x4</code></td>
            <td><code>zhandler</code><br/>I/O channel handler</td>
            <td><code>uint32</code><br/>std channel (0, 1 or 2)</td>
            <td colspan="4"></td>
            <td><code>zhandler</code><br/>old std handler</td>
            <td>Replace a standard I/O channel (in, out, err)</td>
        </tr>
        <tr>
            <td>errno</td>
            <td><code>0x5</code></td>
            <td colspan="6"></td>
            <td><code>uint32</code><br/>latest errcode</td>
            <td>Get latest error code</td>
        </tr>
        <tr>
            <td>alloc</td>
            <td><code>0x6</code></td>
            <td><code>size_t</code><br/>sizeof</td>
            <td><code>flag_t</code><br/>flags</td>
            <td><code>void*</code><br/>realloc</td>
            <td colspan="3"></td>
            <td><code>void*</code><br/>alloc_t</td>
            <td>Alloc memory</td>
        </tr>
        <tr>
            <td colspan="10"><i>Work in progress...</i></td>
        </tr>
    </tbody>
</table>

### C Programming : how work functions' code ?
