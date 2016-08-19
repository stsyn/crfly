FOR %%N in (*.txt) do ( 
patternr.exe %%N exp/%%N
)
cd exp
ren *.txt *.dat
pause