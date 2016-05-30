aw = 1
ah = 1
class MRectangle:
	x = -1
	y = -1
	def __str__(self):
		return "(x: %s, y: %s, width: %s, height: %s)" % (self.x, self.y, \
	 		self.width, self.height)
	         
	def __repr__(self):		
	 	return "(x: %s, y: %s, width: %s, height: %s)\n" % (self.x, self.y, \
	 		self.width, self.height)  

	def __init__(self, width=None, height=None,x=-1,y=-1):
		self.width = width
		self.height = height
		self.x = x
		self.y = y

class SAreaReg:
	def __str__(self):
		return "(x: %s, y: %s, width: %s, height: %s, isFree: %s)" % (self.x, self.y, \
	 		self.width, self.height, self.isFree)
	         
	def __repr__(self):		
	 	return "(x: %s, y: %s, width: %s, height: %s, isFree: %s)\n" % (self.x, self.y, \
	 		self.width, self.height, self.isFree)        

	def __init__(self,x=None,y=None,width=None,height=None,isFree=None):
		self.x = x
		self.y = y
		self.width = width
		self.height = height
		self.isFree = isFree

def findHeightStep(i,j,rect,listArea,area):
	if area.height >= rect.height:
		return 0
	else:
		needHeight = rect.height
		currentHeight = area.height
		t = i + 1
		bottomArea = None
		if t<ah:			
			bottomArea = listArea[t][j]
		stepHeight = 0
		while bottomArea!=None and bottomArea.isFree==True and currentHeight<needHeight:
			stepHeight+=1
			currentHeight = currentHeight+bottomArea.height
			if currentHeight>=needHeight:
				return stepHeight
			else:
				t+=1
				if t<ah: 	
					bottomArea = listArea[t][j]
				else:
					bottomArea = None

	return -1

def hasStepHeight(i, j, stepHeight, listArea ):
	for step in range(1,stepHeight+1):
		if listArea[i+step][j].isFree == False: 
			return False
	return True

def findWidthStep(i,j,rect,listArea,area,heightStep):
	if area.width >= rect.width:
		return 0
	else:
		needWidth = rect.width
		currentWidth = area.width
		t = j+1
		stepWidth = 0
		rightArea = None
		if t<aw: 			
			rightArea = listArea[i][t]
		while rightArea!=None and rightArea.isFree==True and currentWidth<needWidth \
		and hasStepHeight(i,t,heightStep,listArea):
			stepWidth+=1
			currentWidth += rightArea.width
			if(currentWidth>=needWidth): return stepWidth
			else:
				t+=1
				if t<aw: rightArea = listArea[i][t]
				else: rightArea = None
	return -1

def findPlace(rect,listArea,enclosingRect):
	for j in range(aw):
		for i in range(ah):
			area = listArea[i][j]
			if area.x + rect.width > enclosingRect.width:
				return (-1,-1,-1,-1,-1,-1)
			if area.isFree == True:
				heightStep = findHeightStep(i,j,rect,listArea,area) 
				if(heightStep>=0):
					widthStep = findWidthStep(i,j,rect,listArea,area,heightStep) 
					if widthStep>=0:
						return(area.x,area.y,i,j,heightStep+i,widthStep+j)
	return (-1,-1,-1,-1,-1,-1)

def place(rect,x,y):
	rect.x = x
	rect.y = y

def updateListArea(listArea,rect,brow,bcol,erow,ecol):
	area = listArea[0][ecol]	
	maxWidthPos = area.x+area.width
	maxRectWidthPos = rect.x+rect.width
	w2 = maxWidthPos - maxRectWidthPos
	if(w2>0):		
		w1 = maxRectWidthPos - area.x		
		global aw
		global ah
		aw+=1		
		for i in range(ah):
			area2 = listArea[i][ecol]
			area2.width = w1
			area = SAreaReg(x=(area2.x+w1),y=area2.y,width=w2,height=area2.height,isFree=area2.isFree)
			listArea[i].insert(ecol+1,area)

	area = listArea[erow][0]
	maxHeightPos = area.y + area.height
	maxRectHeightPos = rect.y + rect.height
	h2 = maxHeightPos - maxRectHeightPos
	if(h2>0):
		h1 = maxRectHeightPos - area.y
		ah+=1
		newRow = []
		for i in range(aw):
			area2 = listArea[erow][i]
			area2.height = h1
			area = SAreaReg(x=area2.x,y=(area2.y+h1),\
				width=area2.width,height=h2,isFree=area2.isFree)
			newRow.append(area)
		listArea.insert(erow+1,newRow)

	for i in range(brow,erow+1):
		for j in range(bcol,ecol+1):
			listArea[i][j].isFree = False

def pack(listRect, enclosingRect):
	listRect.sort(key = lambda x:x.height, reverse = True)
	listArea = [[SAreaReg(0,0,enclosingRect.width,enclosingRect.height,True)]]
	global aw
	global ah
	aw = ah = 1
	for rect in listRect:
		# print ("----dskjdhahdbsjdb--------")
		p = findPlace(rect,listArea,enclosingRect)
		if (p[0]>=0):
			place(rect,p[0],p[1])
			updateListArea(listArea,rect,p[2],p[3],p[4],p[5])
	return listRect

def packTile(listRect,enclosingRect,n,packedAll):
	r = []
	packedAll[0] = False
	unpackedRect = listRect
	for i in range(n):
		t = pack(unpackedRect,enclosingRect)
		packedRect = []
		unpackedRect = []
		
		for j in range(len(t)):
			if(t[j].x != -1):
				packedRect.append(t[j])
			else:				
				unpackedRect.append(t[j])

		r.append(packedRect)
		if len(unpackedRect) == 0 :
			packedAll[0] = True
			break
	return r

# area11 = SAreaReg(x=0,y=0,width=15,height=25,isFree=False)
# area12 = SAreaReg(x=15,y=0,width=20,height=25,isFree=False)
# area13 = SAreaReg(x=35,y=0,width=5,height=25,isFree=True)

# area21 = SAreaReg(x=0,y=25,width=15,height=5,isFree=False)
# area22 = SAreaReg(x=15,y=25,width=20,height=5,isFree=True)
# area23 = SAreaReg(x=35,y=25,width=5,height=5,isFree=True)

# area31 = SAreaReg(x=0,y=30,width=15,height=10,isFree=True)
# area32 = SAreaReg(x=15,y=30,width=20,height=10,isFree=True)
# area33 = SAreaReg(x=35,y=30,width=5,height=10,isFree=True)

# area41 = SAreaReg(x=0,y=40,width=15,height=8,isFree=True)
# area42 = SAreaReg(x=15,y=40,width=20,height=8,isFree=True)
# area43 = SAreaReg(x=35,y=40,width=5,height=8,isFree=True)

# aw = 3
# ah = 4
# listArea = [[area11,area12,area13],[area21,area22,area23],[area31,area32,area33],[area41,area42,area43]]

# -----------------------------test1 ------------------------------------
# enclosingRect = MRectangle(width = 40,height = 40)
# listArea = [[SAreaReg(0,0,enclosingRect.width,enclosingRect.height,True)]]
# aw = ah = 1

# r1 = MRectangle(width=15,height=30)
# p = findPlace(r1,listArea,enclosingRect)
# # print (p)
# place(r1,p[0],p[1])
# updateListArea(listArea,r1,p[2],p[3],p[4],p[5])
# # print (listArea)

# r2 = MRectangle(width=20,height=25)
# p = findPlace(r2,listArea,enclosingRect)
# place(r2,p[0],p[1])
# updateListArea(listArea,r2,p[2],p[3],p[4],p[5])
# # print (r2)
# # print (listArea)

# r5 = MRectangle(width=10,height=10)
# p = findPlace(r5,listArea,enclosingRect)
# place(r5,p[0],p[1])
# updateListArea(listArea,r5,p[2],p[3],p[4],p[5])
# # print (r5)

# r4 = MRectangle(width=20,height=8)
# p = findPlace(r4,listArea,enclosingRect)
# place(r4,p[0],p[1])
# updateListArea(listArea,r4,p[2],p[3],p[4],p[5])
# # print (r4)

# r3 = MRectangle(width=9,height=7)
# p = findPlace(r3,listArea,enclosingRect)
# place(r3,p[0],p[1])
# updateListArea(listArea,r3,p[2],p[3],p[4],p[5])
# # print (r3)

# ----------------------------test2---------------------------
# enclosingRect = MRectangle(width = 37,height = 40)
# r1 = MRectangle(width=15,height=30)
# r2 = MRectangle(width=20,height=25)
# r3 = MRectangle(width=9,height=7)
# r4 = MRectangle(width=20,height=8)
# r5 = MRectangle(width=10,height=10)
# listRect = [r1,r2,r3,r4,r5]
# n = 10
# packedAll = [False]
# r = packTile(listRect,enclosingRect,n,packedAll)

# print (packedAll)


#-------------------------test3----------------------------------
enclosingRect = MRectangle(width = 500,height = 500)
listRect = []

from random import randint
for i in range(2000):
	w = randint(1,30)
	h = randint(1,40)
	r = MRectangle(width=w,height=h)
	listRect.append(r) 

packedAll = [False]
n = 10
r = packTile(listRect,enclosingRect,n,packedAll)

print (listRect)
pack(listRect, enclosingRect)
print (listRect)
