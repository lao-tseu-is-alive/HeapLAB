
# Is Golang compiled binaries subject to GlibC heap attack's ?

To compile a simple hello world in Go just do:

	go build -o gohello gohello.go

then if you want to check the bindings with glibc...

	ldd gohello
	not a dynamic executable


If you check this binary with [checksec](https://github.com/lao-tseu-is-alive/checksec.sh)

<pre>~/bin/checksec --output=json  --file=./gohello |jq
<b>{</b>
<b>  </b><font color="#12488B"><b>&quot;./gohello&quot;</b></font><b>: {</b>
<b>    </b><font color="#12488B"><b>&quot;relro&quot;</b></font><b>: </b><font color="#26A269">&quot;no&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;canary&quot;</b></font><b>: </b><font color="#26A269">&quot;no&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;nx&quot;</b></font><b>: </b><font color="#26A269">&quot;yes&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;pie&quot;</b></font><b>: </b><font color="#26A269">&quot;no&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;rpath&quot;</b></font><b>: </b><font color="#26A269">&quot;n/a&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;runpath&quot;</b></font><b>: </b><font color="#26A269">&quot;n/a&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;symbols&quot;</b></font><b>: </b><font color="#26A269">&quot;yes&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;fortify_source&quot;</b></font><b>: </b><font color="#26A269">&quot;no&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;fortified&quot;</b></font><b>: </b><font color="#26A269">&quot;0&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;fortify-able&quot;</b></font><b>: </b><font color="#26A269">&quot;0&quot;</font>
<b>  }</b>
<b>}</b>
</pre>


you can find information on the history of this security tool in [Tobias Klein web](https://www.trapkit.de/tools/checksec/)
<br><br>

[Stack Canaries are very simple - at the beginning of the function, a random value is placed on the stack](https://ir0nstone.gitbook.io/notes/types/stack/canaries)

[NX bit, wich stands for No eXecute](https://ir0nstone.gitbook.io/notes/types/stack/no-execute)

[PIE stands for Position Independent Executable](https://ir0nstone.gitbook.io/notes/types/stack/pie)

[Information on flags RPATH and RUNPATH](https://security.stackexchange.com/questions/161799/why-does-checksec-sh-highlight-rpath-and-runpath-as-security-issues/165762#165762)


[checksec security properties explained](https://opensource.com/article/21/6/linux-checksec)


### [RELRO: Relocation Read-Only](https://www.trapkit.de/articles/relro/) 
RELRO is a generic exploit mitigation technique to harden the data sections of an Executable and Linkable Format (ELF) binary or process. RELRO has two different modes, which are described in detail below.

  #### Partial RELRO:
simply moves the GOT (Global Offset Table) above the program's variables, meaning you can't overflow into the GOT. This, of course, does not prevent format string overwrites.

 + Compiler command line: **gcc -Wl,-z,relro.**
 + The ELF sections are reordered so that the ELF internal data sections (.got, .dtors, etc.) precede the program's data sections (.data and .bss).
 + Non-PLT Global Offsets Table (GOT) is read-only.
+ PLT-dependent GOT is still writeable.


  #### Full RELRO:
makes the GOT  (Global Offset Table) completely read-only, so even format string exploits cannot overwrite it. This is not the default in binaries due to the fact that it can make it take much longer to load as it need to resolve all the function addresses at once.
 + Compiler command line: **gcc -Wl,-z,relro,-z,now.**
 + Supports all the features of Partial RELRO.
 + Bonus: The entire GOT is also (re)mapped as read-only.


### [PIE Position Independent Code](https://ir0nstone.gitbook.io/notes/types/stack/pie)
PIE stands for Position Independent Executable, which means that every time you run the file it gets loaded into a different memory address. 
This means you cannot hardcode values such as function addresses and gadget locations without finding out where they are.

### how to get RELRO & PIE on a Go binary

To be able to get RELRO protection you can just add **[-buildmode=pie](https://pkg.go.dev/cmd/go@go1.21.0#hdr-Build_modes)** to 
the Go build command. You will get the added benefit of activating [PIE (position independent executables)](https://ir0nstone.gitbook.io/notes/types/stack/pie)

This will add ~100Kb of code to your binary

    go build -buildmode=pie   -o gohello gohello.go

<pre> ~/bin/checksec --output=json  --file=./gohello |jq
<b>{</b>
<b>  </b><font color="#12488B"><b>&quot;./gohello&quot;</b></font><b>: {</b>
<b>    </b><font color="#12488B"><b>&quot;relro&quot;</b></font><b>: </b><font color="#26A269">&quot;partial&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;canary&quot;</b></font><b>: </b><font color="#26A269">&quot;no&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;nx&quot;</b></font><b>: </b><font color="#26A269">&quot;yes&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;pie&quot;</b></font><b>: </b><font color="#26A269">&quot;yes&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;rpath&quot;</b></font><b>: </b><font color="#26A269">&quot;no&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;runpath&quot;</b></font><b>: </b><font color="#26A269">&quot;no&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;symbols&quot;</b></font><b>: </b><font color="#26A269">&quot;yes&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;fortify_source&quot;</b></font><b>: </b><font color="#26A269">&quot;no&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;fortified&quot;</b></font><b>: </b><font color="#26A269">&quot;0&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;fortify-able&quot;</b></font><b>: </b><font color="#26A269">&quot;0&quot;</font>
<b>  }</b>
<b>}</b>
</pre>

### how to get rid of the symbols in the Go binary without using strip

by passing the -ldflags "-s -w" you can remove the symbols in the same way as using strip. As added benefit 
the size of the binary wil go down to  1332 Ko from the 1944 ko at the previous step.

    CGO_ENABLED=0 GOOS=linux go build -buildmode=pie  -ldflags "-s -w"   -o gohello gohello.go

<pre>~/bin/checksec --output=json  --file=./gohello |jq
<b>{</b>
<b>  </b><font color="#12488B"><b>&quot;./gohello&quot;</b></font><b>: {</b>
<b>    </b><font color="#12488B"><b>&quot;relro&quot;</b></font><b>: </b><font color="#26A269">&quot;partial&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;canary&quot;</b></font><b>: </b><font color="#26A269">&quot;no&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;nx&quot;</b></font><b>: </b><font color="#26A269">&quot;yes&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;pie&quot;</b></font><b>: </b><font color="#26A269">&quot;yes&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;rpath&quot;</b></font><b>: </b><font color="#26A269">&quot;no&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;runpath&quot;</b></font><b>: </b><font color="#26A269">&quot;no&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;symbols&quot;</b></font><b>: </b><font color="#26A269">&quot;no&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;fortify_source&quot;</b></font><b>: </b><font color="#26A269">&quot;no&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;fortified&quot;</b></font><b>: </b><font color="#26A269">&quot;0&quot;</font><b>,</b>
<b>    </b><font color="#12488B"><b>&quot;fortify-able&quot;</b></font><b>: </b><font color="#26A269">&quot;0&quot;</font>
<b>  }</b>
</pre>


