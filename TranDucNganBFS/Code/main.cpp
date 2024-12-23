#include <stdio.h>
#include <math.h>
#include <queue>
#include <graphics.h>

#define MAX_SIZE 100
#define M_PI 3.14159265358979323846

//Khai bao ham
void drawframe();
void drawButton(int left, int top, int right, int bottom, const char* label);
bool isMouseHover(int left, int top, int right, int bottom);
void handleMouseHover(int left, int top, int right, int bottom, const char* label, bool &hovered);
void drawgraph();
void clearGraph();
void cleanGraph();
void bfs(int current, int end, int n, int& done);
void BFS_display();
void openAndReadFile(const char* filePath);
void handleClick(int x, int y);
void mouse();
void waitForKeypress();

//Bien danh dau
bool isResetMode = false;//Bien danh dau reset
bool isGraphLoaded = false;//Bien xac dinh xem do thi da duoc load hay chua 
bool isBFSExecuted = false;// Bien check bfs da chay lan nao chua

int visited[MAX_SIZE];
int parent[MAX_SIZE];
int path[MAX_SIZE];
int len = 0;

//do thi
int numV; //so dinh
struct Coord { //toa do
    int x;
    int y;
    int label;
};
struct Coord vertices[MAX_SIZE];
FILE *fp;
char buffer[10];
int adjMatrix[MAX_SIZE][MAX_SIZE]; //ma tran ke

//Doc file toa do va ma tran ke
void openAndReadFile(const char* filePath) {
    fp = fopen(filePath, "r");
    if (fp == NULL) {
        printf("File not found!");
    }
	else {
        fscanf(fp, "%d", &numV);
        for (int i = 0; i < numV; i++) {
            fscanf(fp, "%d %d %d", &vertices[i].x, &vertices[i].y, &vertices[i].label);
        }
    }
    for (int i = 0; i < numV; i++) {
        for (int j = 0; j < numV; j++) {
            fscanf(fp, "%d", &adjMatrix[i][j]);
        }
    }
    isGraphLoaded = true; //Dat co gia tri la true khi do thi duoc load
    fclose(fp);
}

// Ham ve mui ten
void drawArrow(int x1, int y1, int x2, int y2) {
    const int arrow_size = 10; // Kich thuoc cua mui ten

    // Tinh goc giua hai diem
    double angle = atan2(y2 - y1, x2 - x1);

    // Tinh toan cac diem cua mui ten
    int x3 = x2 - arrow_size * cos(angle - M_PI / 6);
    int y3 = y2 - arrow_size * sin(angle - M_PI / 6);
    int x4 = x2 - arrow_size * cos(angle + M_PI / 6);
    int y4 = y2 - arrow_size * sin(angle + M_PI / 6);

    // Ve duong thang
    line(x1, y1, x2, y2);

    // Ve mui tên
    line(x2, y2, x3, y3);
    line(x2, y2, x4, y4);
}

// Ham ve do thi
void drawgraph() {
    const int RADIUS = 20; // Ban kinh cua vong tron cho cac dinh
    settextstyle(1, 0, 3);

    // Ve các dinh
    for (int i = 0; i < numV; i++) {
        circle(vertices[i].x, vertices[i].y, RADIUS);
        outtextxy(vertices[i].x - 10, vertices[i].y - 10, itoa(vertices[i].label, buffer, 10));
    }

    // Ve cac canh
    for (int i = 0; i < numV; i++) {
        for (int j = 0; j < numV; j++) {
            if (adjMatrix[i][j] == 1 && adjMatrix[j][i] == 0) {  // Ve mui ten tu i den j (mot chieu)
                // Tinh goc giua 2 dinh
                double angle = atan2(vertices[j].y - vertices[i].y, vertices[j].x - vertices[i].x);

                // Tính toa do diem tiep tuyen trên chu vi cua các dinh
                int x1 = vertices[i].x + RADIUS * cos(angle);
                int y1 = vertices[i].y + RADIUS * sin(angle);
                int x2 = vertices[j].x - RADIUS * cos(angle);
                int y2 = vertices[j].y - RADIUS * sin(angle);

                // Ve mui tên mot chieu tu i den j
                drawArrow(x1, y1, x2, y2);
            }

            if (adjMatrix[j][i] == 1 && adjMatrix[i][j] == 0) {  // Ve mui tên tu j den i (mot chieu)
                // Tính góc giua 2 dinh
                double angle = atan2(vertices[i].y - vertices[j].y, vertices[i].x - vertices[j].x);

                // Tính toa do diem tiep tuyen trên chu vi cua các dinh
                int x1 = vertices[j].x + RADIUS * cos(angle);
                int y1 = vertices[j].y + RADIUS * sin(angle);
                int x2 = vertices[i].x - RADIUS * cos(angle);
                int y2 = vertices[i].y - RADIUS * sin(angle);

                // Ve mui tên mot chieu tu j den i
                drawArrow(x1, y1, x2, y2);
            }

            // Truong hop có canh hai chieu, ve ca hai mui tên
            if (adjMatrix[i][j] == 1 && adjMatrix[j][i] == 1) {
                // Ve mui tên tu i den j
                double angle1 = atan2(vertices[j].y - vertices[i].y, vertices[j].x - vertices[i].x);
                int x1 = vertices[i].x + RADIUS * cos(angle1);
                int y1 = vertices[i].y + RADIUS * sin(angle1);
                int x2 = vertices[j].x - RADIUS * cos(angle1);
                int y2 = vertices[j].y - RADIUS * sin(angle1);
                drawArrow(x1, y1, x2, y2);

                // Ve mui ten tu j den i
                double angle2 = atan2(vertices[i].y - vertices[j].y, vertices[i].x - vertices[j].x);
                x1 = vertices[j].x + RADIUS * cos(angle2);
                y1 = vertices[j].y + RADIUS * sin(angle2);
                x2 = vertices[i].x - RADIUS * cos(angle2);
                y2 = vertices[i].y - RADIUS * sin(angle2);
                drawArrow(x1, y1, x2, y2);
            }
        }
    }
}

//Ham xu ly thao tac
void handleClick(int x, int y) {
	//in4
    if(x >= 567 && x <= 700 && y >= 10 && y <= 50){
        cleanGraph(); 
        drawframe();

        settextstyle(10, 0, 2);
        setcolor(RED);
        outtextxy(80, 50, "BFS Visualization for Graphs");

        settextstyle(8, 0, 1);
        setcolor(WHITE);
        outtextxy(80, 110, "Supervisor: M.Sc. DOAN VU THINH");
        outtextxy(80, 160, "Student: TRAN DUC NGAN");
        outtextxy(80, 210, "Student ID: 64131460");

        settextstyle(6, 0, 1);
        setcolor(YELLOW);        
        outtextxy(80, 270, "Thank you for reviewing the project!");
    }
	
	//Open file
	if(x >= 567 && x <= 700 && y >= 70 && y <= 110){
		OPENFILENAME ofn;
		TCHAR szFile[MAX_PATH];
		TCHAR szFolder[MAX_PATH];
		//Lay duong dan den thu muc hien tai cua chuong trinh
		GetCurrentDirectory(MAX_PATH, szFolder);
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = TEXT("All Files (*.*)\0*.*\0");
		ofn.nFilterIndex = 1;

		ofn.lpstrInitialDir = szFolder; //Su dung thu muc hien tai lam thu muc mac dinh
    	if (GetOpenFileName(&ofn)) {
        	const char* filePath = ofn.lpstrFile;
        	openAndReadFile(filePath);
        	ZeroMemory(szFile, sizeof(szFile));
		}
		clearGraph();
        drawframe();
        drawgraph();
	}

	//Start
    if (x >= 567 && x <= 700 && y >= 130 && y <= 170) {	
    	//Neu do thi da duoc load hoac dang o che do reset
    	if(isGraphLoaded || isResetMode ){
        	//Chay BFS neu chua duoc chay
        	if (!isBFSExecuted) {
            	BFS_display(); // Chay bfs
            	isBFSExecuted = true; // Da chay bfs
        	} else {
            	cleanGraph();
            	drawframe();
            	drawgraph();
            	BFS_display();
        	}
        	isGraphLoaded=true;
    	}else{
    		setcolor(WHITE); 
    		cleanGraph();  //Xoa do thi
			drawframe();   //Ve khung
    		outtextxy(195, 120, "NO GRAPH TO START");  //Khong co do thi de bat dau
    		outtextxy(60, 170, "CLICK THE 'Load file' BUTTON TO ADD GRAPH");  //Click vao nut 'Load file' de them do thi
		}
	}
	
    //Reset
    if (x >= 567 && x <= 700 && y >= 190 && y <= 230) {
    	//Neu do thi da duoc load
    	if (isGraphLoaded) {
        	isResetMode = true;
        	isBFSExecuted = false;
        	cleanGraph();  //Xoa do thi
        	drawframe();   //Ve khung
        	drawgraph();   //Ve do thi moi
    	}
    	isGraphLoaded = false;
	}
    
    //Delete
    if (x >= 567 && x <= 700 && y >= 250 && y <= 290) {
    	isResetMode = false;
        cleanGraph();  //Xoa do thi
        drawframe();   //Ve khung
        outtextxy(180, 120, "DATA HAS BEEN DELETED");  //Du lieu da bi xoa
    	outtextxy(60, 170, "CLICK THE 'Load file' BUTTON TO ADD GRAPH");  //Click vao nut 'Load file' de them do thi
    }

    //Exit
    if (x >= 567 && x <= 700 && y >= 310 && y <= 350) {
        closegraph();  //Dong cua so do thi
    }
}


//Kiem tra hover
bool isMouseHover(int left, int top, int right, int bottom) {
    int x = mousex();
    int y = mousey();
    if (x >= left && x <= right && y >= top && y <= bottom) {
        return true; // Chuot di chuyen vao trong vung cua button
    }
    return false; // Chuot di chuyen ra khoi vung cua button
}

//Xu ly khi hover vao button
void handleMouseHover(int left, int top, int right, int bottom, const char* label, bool &hovered) {
    char buffer[MAX_SIZE]; // Khai báo buffer luu du lieu chuoi
    if (isMouseHover(left, top, right, bottom)) {
        setcolor(RED);
        rectangle(left, top, right, bottom);
        settextstyle(10, 0, 1);

        sprintf(buffer, "%s", label); //Dinh dang chuoi duoc luu vao buffer
        outtextxy(left + 10, top + 10, buffer); // Hien thi noi dung cua buffer

        hovered = true;
    } else if (hovered) {
        setcolor(WHITE);
        rectangle(left, top, right, bottom);
        settextstyle(10, 0, 1);
        
        sprintf(buffer, "%s", label); 
        outtextxy(left + 10, top + 10, buffer);
        hovered = false;
    }
}

//Ham ve button
void drawButton(int left, int top, int right, int bottom, const char* label) {
    setcolor(WHITE);
	setlinestyle(0, 1, 3);
    rectangle(left, top, right, bottom);
    settextstyle(10, 0, 1);

    char buffer[MAX_SIZE];
    strcpy(buffer, label);
    outtextxy(left + 10, top + 10, buffer);
}


//Ve khung do hoa
void drawframe() {
    setcolor(WHITE);
    setlinestyle(0, 1, 3);
    rectangle(10, 10, 550, 370);
    
    rectangle(10, 380, 550, 480);
    settextstyle(10, 0, 1);
    
    drawButton(567, 10, 700, 50, "Infor");
	drawButton(567, 70, 700, 110, "Load file");
    drawButton(567, 130, 700, 170, "Start");
    drawButton(567, 190, 700, 230, "Reset");
    drawButton(567, 250, 700, 290, "Delete");	
	drawButton(567, 310, 700, 350, "Exit");
}


//Xoa do thi
void clearGraph(){
	isBFSExecuted=false;
	memset(visited, 0, sizeof(visited));
	memset(path, 0, sizeof(path));
	len = 0;
	cleardevice();
}

//Xoa do thi
void cleanGraph(){
	memset(visited, 0, sizeof(visited));
	memset(path, 0, sizeof(path));
	len = 0;
	cleardevice();
	isGraphLoaded = false;//Gan ve false sau khi xoa do thi the hien chua nap
}

void bfs(int start, int end, int n) {
    if (start < 0 || start >= n || end < 0 || end >= n) {
    	setcolor(WHITE);
    	settextstyle(10, 0, 1);
        outtextxy(20, 390, "INVALID START OR END NODE");
        return;
    }

    std::queue<int> q;
    memset(visited, 0, sizeof(visited));
    memset(parent, -1, sizeof(parent));

    q.push(start);
    visited[start] = 1;

    // Hien thi duong tìm kiem BFS
    while (!q.empty()) {
        int current = q.front();
        q.pop();

        setcolor(RED);
        circle(vertices[current].x, vertices[current].y, 20);
        delay(500);

        if (current == end) {
            break;
        }

        for (int i = 0; i < n; i++) {
            if (adjMatrix[current][i] == 1 && !visited[i]) {
                q.push(i);
                visited[i] = 1;
                parent[i] = current;

                // Tính góc giua hai dinh
                double angle = atan2(vertices[i].y - vertices[current].y, vertices[i].x - vertices[current].x);

                // Tính diem tiep tuyen trên chu vi
                int x1 = vertices[current].x + 20 * cos(angle); // Ðiem tiep tuyen tu dinh current
                int y1 = vertices[current].y + 20 * sin(angle);
                int x2 = vertices[i].x - 20 * cos(angle); // Ðiem tiep tuyen tu dinh i
                int y2 = vertices[i].y - 20 * sin(angle);

                // Ve duong noi tu diem tiep tuyen và mui tên màu do
                setcolor(RED);
                drawArrow(x1, y1, x2, y2);

                delay(400);
            }
        }
    }

    // Hien thi duong di
    if (visited[end]) {
        // Xây dung duong di
        int path[MAX_SIZE];
        int len = 0;
        for (int v = end; v != -1; v = parent[v]) {
            path[len++] = v;
        }

        setcolor(YELLOW);  // Duong di mau vang
        for (int i = len - 1; i > 0; i--) {
            // Tinh góc giua hai dinh trong duong di
            double angle = atan2(vertices[path[i]].y - vertices[path[i - 1]].y, vertices[path[i]].x - vertices[path[i - 1]].x);

            // Tính diem tiep tuyen trên chu vi
            int x1 = vertices[path[i]].x - 20 * cos(angle);
            int y1 = vertices[path[i]].y - 20 * sin(angle);
            int x2 = vertices[path[i - 1]].x + 20 * cos(angle);
            int y2 = vertices[path[i - 1]].y + 20 * sin(angle);

            // Ve lai duong mui ten
            drawArrow(x1, y1, x2, y2);
            delay(200);
        }

        for (int i = 0; i < len; i++) {
            setcolor(YELLOW);
            circle(vertices[path[i]].x, vertices[path[i]].y, 20);
            delay(100);
        }
		
		settextstyle(10, 0, 1);
		setcolor(WHITE);
		outtextxy(20, 390, "PATH: ");
		for (int i = len - 1; i >= 0; i--) {
		    sprintf(buffer, "%d", path[i]);
		    outtextxy(120 + (len - 1 - i) * 30, 390, buffer);
		}
	}
	else {
		settextstyle(10, 0, 1);
		setcolor(WHITE);
        outtextxy(20, 390, "NO PATH FOUND");
    }
}

void BFS_display() {
    int start, end;
    printf("Enter the starting vertex: ");
    scanf("%d", &start);
    printf("Enter the ending vertex: ");
    scanf("%d", &end);
    bfs(start, end, numV);
}

//Ham xu ly con tro chuot
void mouse() {
    int x;
    int y;
    //Bien danh dau hover
    bool hovered = false;
	bool hovered1 = false; 
	bool hovered2 = false; 
	bool hovered3 = false; 
	bool hovered4 = false; 
	bool hovered5 = false; 
    while (!kbhit()) {
        if (ismouseclick(WM_MOUSEMOVE)) {
            getmouseclick(WM_MOUSEMOVE, x, y);
            handleMouseHover(567, 10, 700, 50, "Infor", hovered);
            handleMouseHover(567, 70, 700, 110, "Load file", hovered1);
            handleMouseHover(567, 130, 700, 170, "Start", hovered2);
            handleMouseHover(567, 190, 700, 230, "Reset", hovered3);
            handleMouseHover(567, 250, 700, 290, "Delete", hovered4);
            handleMouseHover(567, 310, 700, 350, "Exit", hovered5);
            clearmouseclick(WM_MOUSEMOVE);
        }
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            handleClick(x, y);
            clearmouseclick(WM_LBUTTONDOWN);
        }
    }
}

//main
int main() {
    initwindow(720, 500);
    drawframe();
    mouse();
    
    getch();
    closegraph();
    
    return 0;
}
