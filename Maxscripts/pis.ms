function putVrmat2Obj geos mat = 
(
	for g in geos do
	(
		mclass = classof g.material
		if (mclass == StandardMaterial) then
		(
			
			if g.material.name == mat.name then	
			(
				g.material = mat	
			)			
		) 
		else if mclass == MultiMaterial then 
		(
			for a = 1 to g.material.count do
			(
				if (classof g.material.materialList[a] == StandardMaterial) and (g.material.materialList[a].name == mat.name) then
				(
					g.material.materialList[a] = mat
				)
			)
		)
		else (
			--print mclass
		)

	)	
)

for i in currentMaterialLibrary do
(	
	putVrmat2Obj geometry i
)


