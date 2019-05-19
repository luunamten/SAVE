using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;


namespace GameCs
{

    //lop xu ly game
    class CentraProccessing
    {
        public readonly Poster firstPoster; //cac mau khung menu
        public readonly Poster blankPoster;
        public readonly Poster win;

        int speed;                         //toc do game
        bool isRun;                        //dieu kien chay va ket thuc thread
        InfoTable info;                     
        Stack<Activity> activity;
        Activity topOfStack;                //stack chua cac activity
        Stack<Activity> notification;
        Stack<string[]> sourceMap;          //danh sach cac ban do choi che do chien dich
        TypePlay type;                      //kieu choi (chien dich, tu do)
        
        public enum TypePlay {
            TUDO, CHIENDICH
        }
        public CentraProccessing()
        {
            Console.SetWindowSize(Game.W_WIDTH,Game.W_HEIGHT);
            Console.BackgroundColor = Game.W_COLOR;
            Game.mapColor = ConsoleColor.Yellow;
            Game.snake_color =ConsoleColor.Cyan;
            Game.snake_fig = '▓';
            firstPoster = new Poster("poster/poster.mp");
            blankPoster = new Poster("poster/menu.mp");
            win = new Poster("poster/Win.mp");
            info = new InfoTable("control/control.mp");          
            info.showFrame();

            notification = new Stack<Activity>();
            sourceMap = new Stack<string[]>();
            activity = new Stack<Activity>();          
            Poster p = new Poster(firstPoster);
            PosterActivity s = new PosterActivity(p,this,"Poster");
            activity.Push(s);
            topOfStack = activity.First();
            topOfStack.work();
        }

        //truyen thong tin vao bang info
        public void addInfomation(InfoTable.TYPE type, string data, ConsoleColor color)
        {
            info.addData(type,data,color);
        }

        //dua activity vao stack
        public void pushStack(Activity a)
        {
            activity.Push(a);
            topOfStack = a;
        }

        //lay activity ra khoi stack
        public void popStack()
        {
            if (activity.Count() == 0) return;
            activity.Pop();
            topOfStack = activity.First();
        }

        //tao cac duong dan chua ban do chien dich
        private void createMapSourceCD()
        {
            if(sourceMap.Count()!=0)
            {
                sourceMap.Clear();
            }
            string[] s1 =
            {
                "Level 1",
                "mapcd/No Name.mp"
            };
            string[] s2 =
           {
                "Level 2",
                "mapcd/Crate.mp"
            };
            string[] s3 =
            {
                "Level 3",
                "mapcd/Bad Route.mp"
            };
            string[] s4 =
           {
                "Level 4",
                "mapcd/Circle.mp"
            };
            string[] s5 =
            {
                "Level 5",
                "mapcd/Walls.mp"
            };
            string[] s6 =
           {
                "Level 6",
                "mapcd/T-Tree.mp"
            };
            string[] s7 = {
                "Level 7",
                "mapcd/Spiral.mp" };
            sourceMap.Push(s7);
            sourceMap.Push(s6);
            sourceMap.Push(s5);
            sourceMap.Push(s4);
            sourceMap.Push(s3);
            sourceMap.Push(s2);
            sourceMap.Push(s1);
        }
        public void createEndGame()
        {
            if(notification.Count()!=0)
            {
                notification.Clear();
            }
            string[] s1 = {
                            "Made by L.P.P.N 12/2016",
                            "Cau truc du lieu va giai thuat",
                            "K15 Hcmute"
                            
                        };
            string[] s2 =
            {
                            "Cam on ban da choi game",
                            "The end."
                        };
            EndGame no1 = new EndGame(win, this, "The End", null);
            EndGame no2 = new EndGame(blankPoster, this, "The End", s1);
            EndGame no3 = new EndGame(blankPoster, this, "The End", s2);
            notification.Push(no3);
            notification.Push(no2);
            pushStack(no1);
        }
        //tao ban do che do choi tu do
        public void createMapSourceTD(string label, string source)
        {
            string[] s = { label,source};
            sourceMap.Push(s);
        }

        //xoa duong dan ban do chien dich khoi danh sach
        public void popMap()
        {
            if (sourceMap.Count() == 0) return;
            sourceMap.Pop();
        }
        //ban do ke tiep
        public string[] getNextMap()
        {
            if (sourceMap.Count != 0)
                return sourceMap.First();
            else return null;
        }
        public Activity getNextNotification()
        {
            if (notification.Count() == 0) return null;
            return notification.First();
        }
        public void popNotification()
        {
            if (notification.Count() == 0) return;
            notification.Pop();
        }

        //bat dau game
        public void beginDrawing()
        {
            Thread t = null;
            if (type == TypePlay.TUDO)
            {
                string[] map = getNextMap();
                Map m = new Map(map[1]);
                User user = new User();
                SingleMapActivity da = new SingleMapActivity(user, m, this, map[0]);
                pushStack(da);
                topOfStack.workFirst();
                t = new Thread(drawTD);
            }
            else
            {
                createMapSourceCD();
                string[] map = getNextMap();
                popMap();
                Map m = new Map(map[1]);
                User user = new User();
                MultiMapActivity da = new MultiMapActivity(user, m, this, map[0]);
                pushStack(da);
                topOfStack.workFirst();
                t = new Thread(drawCD);
            }
            isRun = true;
            t.Start();
        }

        //game thread
        private void drawTD()
        {
            while (isRun)
            {
                
                topOfStack.work(); 
                Thread.Sleep(speed);               
            }
        }
        private void drawCD()
        {
           while(isRun)
            {
                topOfStack.work();
                Thread.Sleep(speed);                 
            }
        }


        //lay du lieu tu ban phim
        public void OnPress(char key)
        {
            topOfStack.getKey(key);
        }

        public void setSpeed(int speed)
        {
            this.speed = speed;
        }

        public void setTypePlay(TypePlay type)
        {
            this.type = type;
        }
        public void clearInfo()
        {
            info.clear();
        }

        //hien thi activity dung dau cua stack
        public void drawTopOfStack()
        {
            topOfStack.drawAll() ;
        }
        public void topOfStackWork()
        {
            topOfStack.workFirst();
        }

        //ve ban thong tin
        public void drawInfoFrame()
        {
            info.showFrame();
        }


        public void killThread()
        {
            isRun = false;
        }

        //xoa du lieu tren ban thong tin
        public void emptySourceMap()
        {
            sourceMap.Clear();
        }
        public TypePlay getPlayType()
        {
            return type;
        }
        public void showAllInfo()
        {
            info.showAll();
        }
    }
}
