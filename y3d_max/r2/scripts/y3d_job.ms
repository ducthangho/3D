(

	rollout yf_job_list "Job manager" width:320 height:401
	(
		dropDownList ddlTasks "Task" pos:[13,9] width:110 height:40
		listBox lbxJobs "Jobs" pos:[15,81] width:110 height:7
		groupBox grp1 "Job detail" pos:[136,10] width:174 height:366
		listBox lbxJObjects "Objects" pos:[150,80] width:145 height:9
		button btn1 "Button" pos:[110,1] width:0 height:0
		editText edtJName "" pos:[185,42] width:107 height:22
		label lbl1 "Name" pos:[151,45] width:34 height:20
		button btnAddTask "+" pos:[15,53] width:22 height:22
		button btnDelTask "-" pos:[40,53] width:22 height:22
		button btnAddObj "+" pos:[150,230] width:22 height:22
		button btnDelObj "-" pos:[175,230] width:22 height:22
		button btnSAllObj "Select All" pos:[201,230] width:75 height:22
		button btnAddJob "+" pos:[17,200] width:22 height:22
		button btnDelJob "-" pos:[42,200] width:22 height:22
		button btnJSelectFace "Select Faces" pos:[15,234] width:86 height:22 enabled:true
		button btnSaveXRef "Save XRef" pos:[15,260] width:86 height:22 enabled:true
		listBox lbxUVList "Unwrap Modifiers" pos:[151,267] width:110 height:3
		button btnJEditUV "Edit UVMap" pos:[151,334] width:86 height:22 enabled:true
		global doAddTask
		fn doAddTask tname =
			(
				MessageBox tName
			)

		on yf_job_list open do
			(
				ddlTasks.items = TaskList.allTaskText()
				ddlTasks.selection = TaskList.currentT
				lbxJobs.items = currentTask.allJobText()
				lbxJobs.selection = currentTask.currentJ
			)

		on lbxJobs selected sel do
			print "hi"
		on lbxJObjects selected sel do
			print "hi"
		on edtJName entered text do
			print "hi"
		on btnAddTask pressed  do
			(
				-- gVarTmp = "Task"+((TaskList.tasks.count+1) as string)
				createDialog yf_input_name
				yf_input_name.okCallback="yf_job_list.doAddTask"
			)
		on btnDelTask pressed  do
			print "hi"
		on btnAddObj pressed  do
			print "hi"
		on btnDelObj pressed  do
			print "hi"
		on btnSAllObj pressed  do
			print "hi"
		on btnAddJob pressed  do
			print "hi"
		on btnDelJob pressed  do
			print "hi"
		on btnSaveXRef pressed  do
			print "hi"
		on lbxUVList selected sel do
			print "hi"
		on btnJEditUV pressed  do
			print "hi"
	)

	struct jobClass
		(
			task_id=1,
			job_name,
			objs=#(), -- cac object ban dau chon de gom thanh newObject
			newO, -- doi tuong duoc tao ra tu cac object duoc chon
			impFaces=#{}, -- cac mat duoc chon cua newObject
			oldXref,
			bakeXref,
			status, 
			-- nhom object, tao layer, attach
			fn prepareObj =
				(
					objs = selection
					if objs.count<1 then
						(
							MessageBox "Please select at least one object!" title: "Warning!"
							return false
						)
					else
						(
							jn = getListItemText()
							jobNode = getNodeByName jn
							if (jobNode != undefined) then
								(
									qq = queryBox  ("Job \""+jn+"\" has already existed! Do you want to replace it ?")
									if qq==false then
										(
											return false
										)
									else
										(
											select jobNode
											max group ungroup
											select objs
										)
								)
							group (objs) jn
							jobNode = getNodeByName jn
							append objs jobNode
							select objs

							-- execute ("select $'"+jn + "'") 

						)
					return true
				),
			fn getListItemText =
				(
					return "Task"+(task_id as string)+"_"+job_name
				),

			-- chon cac mat quan trong
			fn beginSelect selectType=
				(

				),
			fn doSelectFace =
				(

				),
			fn updateGlobal =
				(

				),
			fn onCreate = 
				(
					prepareObj()
				)
		)

	struct taskClass
		(
			jobs = #(),
			task_name,
			task_id,
			currentJ = -1,

			fn addJob jName =
				(
					-- iTmp = findItem yJob 
					isExist = false
					for i=1 to jobs.count do
						(
							if jobs[i].job_name==jName then 
								(
									isExist = true
									exit
								)
						)
					if (isExist==false) then
						(
							newJob = jobClass job_name:jName
							append jobs newJob
							currentJ = jobs.count
							currentJob = jobs[currentJ]
						)
					else
						(
							MessageBox "Da co job tuong tu"
						)
				),
			fn getListItemText =
				(
					return "Task"+(task_id as string)+": "+task_name
				),
			fn allJobText =
				(
					items = #()
					for i=1 to jobs.count do
						(
							append items (jobs[i].getListItemText())
						)
					return items
				)
		)


	struct TaskListClass 
		(
			tasks =#(),
			currentT=-1,
			fn openListJobs =
				(
					dpos = GetDialogPos yf_main
					dpos[1] += 330	
					createDialog yf_job_list modal:false pos:dpos
				),		
			fn addTask tName =
				(
					-- iTmp = findItem yJob 
					isExist = false
					for i=1 to tasks.count do
						(
							if tasks[i].task_name==tName then 
								(
									isExist = true
									exit
								)
						)
					if (isExist==false) then
						(
							newTask = taskClass task_name:tName task_id:(tasks.count+1)
							append tasks newTask
							currentT = tasks.count
							currentTask = tasks[currentT]
						)
					else
						(
							MessageBox "Da co Task tuong tu"
						)
				),	
			fn allTaskText =
				(
					items = #()
					for i=1 to tasks.count do
						(
							append items (tasks[i].getListItemText())
						)
					return items
				)			
		)

	-- if yTask==undefined then 
	TaskList = TaskListClass()
	TaskList.addTask "Task1"
	currentTask.addJob "Job1"
)