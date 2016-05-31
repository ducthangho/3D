fn fn_17__open_in_ps =(
	--function to check if the file really exists, otherwise errors will pop up,
	--thx to Michal Mandrysz for this input
	
	fn fileExists fname = (getfiles fname).count != 0
	--
	if (selection.count!=0 and selection[1].material!=undefined)then(
		bitmaps=#()
		sel=selection as array
		for i in sel do (
			if i.material!=undefined then (
				newbitmaps=getclassinstances Bitmaptexture target:i.material
				for b in newbitmaps where (fileExists b.filename) do(
					--fake appendIfUnique for max9
					--because: appendIfUnique bitmaps b; only works with max2009+
					local exists = false;
					for i in bitmaps do(
						if (i == b)then(
							exists = true;
							exit;
						)
					)
					if (exists == false)then(
						append bitmaps b;
					)
				)
			)
		)
		local ps = createOLEobject "Photoshop.Application"
		ps.Visible=true
		for b in bitmaps do(
			ps.open b.filename;
		)
	)
)