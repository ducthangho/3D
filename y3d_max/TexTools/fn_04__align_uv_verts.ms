fn fn_04__align_uv_verts _angle=(
	undo on(
		_angleSnap = _angle as float / (360.0 / _angle as float);
		print("angle snap; "+_angleSnap as string);
		if classof (modPanel.getCurrentObject()) == Unwrap_UVW then(
			-- check if vertex points are selected. otherwise convert
			_uvBase = modPanel.getCurrentObject();
			_uv1 = _uvBase.unwrap;
			_uv2 = _uvBase.unwrap2;
			_uv5 = _uvBase.unwrap5;
			
			local _mode = _uv2.getTVSubObjectMode();
			if (_mode == 2)then(
				
				
				if ((_uv2.getSelectedEdges()).numberset == 1 and _angle != 90)then(
					print("loop selection");
					_uv2.uvEdgeSelect();
				)
				
				_uv2.edgeToVertSelect();
			)else if (_mode == 3)then(
				_uv2.faceToVertSelect();
			)
	
			
			_array = _uv1.getSelectedVertices();
			_selection = #();--new array
			_maxX;_maxY;_minX;_minY;
			
			_nr=1;
			for i in _array do (
				local _pt = _uvBase.getVertexPosition 1 i;
				
				if (_nr > 1) then(
					local _pt2 = _uvBase.getVertexPosition 1 _maxX;
					if (_pt.x > _pt2.x)then(
						_maxX = i;
					)
					_pt2 = _uvBase.getVertexPosition 1 _minX;
					if (_pt.x < _pt2.x)then(
						_minX = i;
					)
					
					_pt2 = _uvBase.getVertexPosition 1 _maxY;
					if (_pt.y > _pt2.y)then(
						_maxY = i;
					)
					_pt2 = _uvBase.getVertexPosition 1 _minY;
					if (_pt.y < _pt2.y)then(
						_minY = i;
					)
				)else(
					_maxX = i;
					_maxY = i;
					_minX = i;
					_minY = i;
				)
				_selection[_nr] = i;
				_nr+=1;
			)
			
			
			print("line up.."+_nr as string+"  /  "+(modPanel.getCurrentObject()).unwrap.getSelectedVertices() as string);
			
			
			
			if (_nr > 1)then(
				--determine if horizontal or vertical flow
				local _a = _uvBase.getVertexPosition 1 _minX;
				local _b = _uvBase.getVertexPosition 1 _maxX;
				local _w = (_b.x - _a.x);
				_a = _uvBase.getVertexPosition 1 _minY;
				_b = _uvBase.getVertexPosition 1 _maxY;
				local _h = (_b.y - _a.y);
				
				local A=1;
				local B=1;
	
				_sel = #{};
				if (_w > _h )then(
					A = _minX;
					B = _maxX;
				)else(
					A = _minY;
					B = _maxY;
				)
				_sel = #{};
				_sel[A] = true;
				_sel[b] = true;
				--_uv1.selectVertices _sel;
	
				
				if (_selection.count > 2)then(
					local ptA = _uvBase.getVertexPosition 1 A;
					local ptB = _uvBase.getVertexPosition 1 B;
					
					local dx = ptB.x - ptA.x;
					local dy = ptB.y - ptA.y;
					local _a = mod ((atan2 dy dx)+4*360) 360;--the angle in degrees
					local _snap = (mod _a _angle as float) as integer;
					if (_snap > (_angle as float/2.0))then(
						_snap = _angle - _snap;
					)
					--print("snap "+_snap as string);
					if (_snap > 0)then(
						--snap to axis
						if(_w > _h )then(
							ptA.y = ptA.y + dy/2;
							ptB.y = ptA.y;
						)else(
							ptA.x = ptA.x + dx/2;
							ptB.x = ptA.x;
						)
						_uvBase.SetVertexPosition 1 A ptA;
						_uvBase.SetVertexPosition 1 B ptB;
					)
					--_angleSnap
					--print("angle "+_a as string+" %90= ("+_snap as string+")");
	
					for i = 1 to _selection.count do(
						if (_selection[i] != A and _selection[i] != B)then(
							local _pt = _uvBase.getVertexPosition 1 _selection[i];
							
							if(_w > _h )then(
								local _m = (ptB.y - ptA.y) / (ptB.x - ptA.x);
								local _x = _pt.x - ptA.x;
								/*if (relative)then(--interpolate position
									_pt.y+= ((_x * _m + ptA.y) - _pt.y)/8; 
								)else(*/
									_pt.y = _x * _m + ptA.y; 
								--)
							)else(
								local _m = (ptB.x - ptA.x) / (ptB.y - ptA.y);
								local _y = _pt.y - ptA.y;
								/*if (relative)then(--interpolate position
									_pt.x+= ((_y * _m + ptA.x) - _pt.x)/8; 
								)else(*/
									_pt.x = _y * _m + ptA.x; 
								--)
							)
							
							_uvBase.SetVertexPosition 1 _selection[i] _pt;
						)
					)
				)else if (_selection.count == 2) then(
					--align just 2 verts
					local ptA = _uvBase.getVertexPosition 1 A;
					local ptB = _uvBase.getVertexPosition 1 B;
					
					local dx = ptB.x - ptA.x;
					local dy = ptB.y - ptA.y;
					local _a = mod ((atan2 dy dx)+4*360) 360;--the angle in degrees
					local _snap = (mod _a _angle as float) as integer;
					if (_snap > (_angle as float/2.0))then(
						_snap = _angle - _snap;
					)
					if (_snap > 0)then(
						--snap to axis
						if(_w > _h )then(
							ptA.y = ptA.y + dy/2;
							ptB.y = ptA.y;
						)else(
							ptA.x = ptA.x + dx/2;
							ptB.x = ptA.x;
						)
						_uvBase.SetVertexPosition 1 A ptA;
						_uvBase.SetVertexPosition 1 B ptB;
					)
				)
			)
			
		)else(
			print("you are not in UV-edit mode!");
		)
	)
)
--
fn_04__align_uv_verts 0;