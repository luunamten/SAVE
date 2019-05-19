using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameCs
{

    //Activity chua cac cai dat trong game
    class SettingMenu : Activity
    {
        //cac toa do cac item
        const int XCL = 15;//25
        const int YCL = 5;
        const int XFI = 40;//55
        const int YFI = 5;
        const int XCT = 60;
        const int YCT = 5;
        const int NOTI1_X = 30;
        const int NOTI1_Y = 19;
        const int NOTI2_X = 30;
        const int NOTI2_Y = 20;
        const int MS_X = XCL - 8;
        const int MS_Y = YCL - 1;
        const int HD_X = XFI - 10;
        const int HD_Y = YFI - 1;
        const int CT_X = XCT - 10;
        const int CT_Y = YFI - 1;
        const int MAU_SAC_SET = 0;
        const int HINH_DANG_SET = 1;
        const int MAP_SET = 2;

        //cac loi chi dan
        const string MAU_SAC = "Mau sac:";
        const string HINH_DANG = "Hinh dang:";
        const string CONTROL = "Mau ban do:";
        const string NOTICE1 = "Nhan 0 de luu lai.";
        const string NOTICE2 = "Nhan 2 de tai lai mau ban do";



        int menuType;
        int clY;
        int fiY;
        int ctY;
        ConsoleColor[] color = { ConsoleColor.Cyan,ConsoleColor.DarkCyan, ConsoleColor.Red,ConsoleColor.DarkRed,
            ConsoleColor.Magenta, ConsoleColor.DarkMagenta,
            ConsoleColor.Green,ConsoleColor.DarkGreen, ConsoleColor.Blue ,ConsoleColor.DarkBlue};
        ConsoleColor[] mapColor = {ConsoleColor.Yellow,ConsoleColor.DarkYellow, ConsoleColor.Cyan,ConsoleColor.DarkCyan, ConsoleColor.Red,ConsoleColor.DarkRed,
            ConsoleColor.Magenta, ConsoleColor.DarkMagenta,
            ConsoleColor.Green,ConsoleColor.DarkGreen, ConsoleColor.Blue ,ConsoleColor.DarkBlue};
        //item
        string[] mapColorLable = { "yellow","Dark Yellow","Cyan", "Dark Cyan", "Red", "Dark Red", "Magenta", "Dark Magenta", "Green", "Dark Green", "Blue", "Dark Blue" };
        string[] itemColor = { "Cyan", "Dark Cyan","Red","Dark Red", "Magenta", "Dark Magenta", "Green","Dark Green", "Blue", "Dark Blue" };
        char[] itemFigure = { '▓','@','.','*','o','O','0','Q','C'};
        Poster poster;

        public SettingMenu(Poster poster, CentraProccessing cpu)
        {        
            st = Game.OUG;
            label = "Setting";
            menuType = 0;
            clY = Game.colorIndex;
            fiY = Game.figureIndex;
            ctY = Game.mapColorIndex;
            this.poster = poster;
            this.cpu = cpu;
        }

        //lay du lieu ban phim nhap vao
        public override void getKey(char key)
        {
            switch (key)
            {
                case Game.UP_KEY:
                    itemUp();
                    break;
                case Game.DOWN_KEY:
                    itemDown();
                    break;
                case Game.LEFT_KEY:
                    if (menuType == 0) menuType = MAP_SET;
                    else menuType--;
                    transMenuType();
                    break;
                case Game.RIGHT_KEY:
                    if (menuType ==MAP_SET) menuType=MAU_SAC_SET;
                    else menuType++;
                    transMenuType();
                    break;
                case Game.ZERO_KEY:
                    Game.snake_color=color[clY];
                    Game.snake_fig = itemFigure[fiY];
                    Game.colorIndex = clY;
                    Game.figureIndex = fiY;
                    Game.mapColorIndex = ctY;
                    Game.mapColor = mapColor[ctY];
                    break;
                case Game.BACK_KEY:
                    if (cpu.getPlayType() == CentraProccessing.TypePlay.TUDO)
                        cpu.emptySourceMap();
                    cpu.popStack();
                    cpu.drawTopOfStack();
                    break;
                case Game.REFRESH_KEY:
                    cpu.drawInfoFrame();
                    drawAll();
                    break;
            }
        }


        //hoat dong lai
        public override void workFirst()
        {
            menuType = 0;
            drawAll();
        }

        //hoat dong
        public override void work()
        {

            drawAll();

        }

        //chuyen doi loai cai dat
        private void transMenuType()
        {
            if (menuType == HINH_DANG_SET)
            {
                Console.ForegroundColor = ConsoleColor.DarkYellow;
                Console.SetCursorPosition(MS_X, MS_Y);
                Console.WriteLine(MAU_SAC);
                Console.ForegroundColor = ConsoleColor.Yellow;
                Console.SetCursorPosition(HD_X, HD_Y);
                Console.WriteLine(HINH_DANG);
                Console.ForegroundColor = ConsoleColor.DarkYellow;
                Console.SetCursorPosition(CT_X, CT_Y);
                Console.WriteLine(CONTROL);
            }
            else if(menuType==MAU_SAC_SET)
            {
                Console.ForegroundColor = ConsoleColor.DarkYellow;
                Console.SetCursorPosition(HD_X, HD_Y);
                Console.WriteLine(HINH_DANG);
                Console.ForegroundColor = ConsoleColor.Yellow;
                Console.SetCursorPosition(MS_X, MS_Y);
                Console.WriteLine(MAU_SAC);
                Console.ForegroundColor = ConsoleColor.DarkYellow;
                Console.SetCursorPosition(CT_X, CT_Y);
                Console.WriteLine(CONTROL);
            }
            else
            {
                Console.ForegroundColor = ConsoleColor.DarkYellow;
                Console.SetCursorPosition(HD_X, HD_Y);
                Console.WriteLine(HINH_DANG);
                Console.ForegroundColor = ConsoleColor.DarkYellow;
                Console.SetCursorPosition(MS_X, MS_Y);
                Console.WriteLine(MAU_SAC);
                Console.ForegroundColor = ConsoleColor.Yellow;
                Console.SetCursorPosition(CT_X, CT_Y);
                Console.WriteLine(CONTROL);
            }
        }

        //di chuyen len
        private void itemUp()
        {
            if (menuType == MAU_SAC_SET)
            {
                Console.ForegroundColor = Game.ITEM_SET_COLOR;
                Console.SetCursorPosition(XCL, YCL + clY);
                Console.WriteLine(itemColor[clY]);
                if (clY > 0) clY--;
                else if (clY == 0) clY = itemColor.Length - 1;
                Console.ForegroundColor = color[clY];
                Console.SetCursorPosition(XCL, YCL + clY);
                Console.WriteLine(itemColor[clY]);
                Console.SetCursorPosition(XFI, YFI + fiY);
                Console.WriteLine(itemFigure[fiY]);
            }
            else if(menuType ==HINH_DANG_SET)
            {
                Console.ForegroundColor = Game.ITEM_SET_COLOR;
                Console.SetCursorPosition(XFI, YFI + fiY);
                Console.WriteLine(itemFigure[fiY]);
                if (fiY > 0) fiY--;
                else if (fiY == 0) fiY = itemFigure.Length - 1;
                Console.ForegroundColor = color[clY];
                Console.SetCursorPosition(XFI, YFI + fiY);
                Console.WriteLine(itemFigure[fiY]);
            } else
            {
                Console.ForegroundColor = Game.ITEM_SET_COLOR;
                Console.SetCursorPosition(XCT, YCT + ctY);
                Console.WriteLine(mapColorLable[ctY]);
                if (ctY > 0) ctY--;
                else if (ctY == 0) ctY = mapColorLable.Length - 1;
                Console.ForegroundColor = mapColor[ctY];
                Console.SetCursorPosition(XCT, YCT + ctY);
                Console.WriteLine(mapColorLable[ctY]);
            }
        }

        //di chuyen xuong
        private void itemDown()
        {
            if (menuType == MAU_SAC_SET)
            {
                Console.ForegroundColor = Game.ITEM_SET_COLOR;
                Console.SetCursorPosition(XCL, YCL + clY);
                Console.WriteLine(itemColor[clY]);
                if (clY < itemColor.Length - 1) clY++;
                else if (clY == itemColor.Length - 1) clY = 0;
                Console.ForegroundColor = color[clY];
                Console.SetCursorPosition(XCL, YCL + clY);
                Console.WriteLine(itemColor[clY]);
                Console.SetCursorPosition(XFI, YFI + fiY);
                Console.WriteLine(itemFigure[fiY]);
            }
            else if(menuType==HINH_DANG_SET)
            {
                Console.ForegroundColor = Game.ITEM_SET_COLOR;
                Console.SetCursorPosition(XFI, YFI + fiY);
                Console.WriteLine(itemFigure[fiY]);
                if (fiY < itemFigure.Length - 1) fiY++;
                else if (fiY == itemFigure.Length - 1) fiY = 0;
                Console.ForegroundColor = color[clY];
                Console.SetCursorPosition(XFI, YFI + fiY);
                Console.WriteLine(itemFigure[fiY]);
            } else
            {
                Console.ForegroundColor = Game.ITEM_SET_COLOR;
                Console.SetCursorPosition(XCT, YCT + ctY);
                Console.WriteLine(mapColorLable[ctY]);
                if (ctY < mapColorLable.Length - 1) ctY++;
                else if (ctY == mapColorLable.Length - 1) ctY = 0;
                Console.ForegroundColor = mapColor[ctY];
                Console.SetCursorPosition(XCT, YCT + ctY);
                Console.WriteLine(mapColorLable[ctY]);
            }
        }


        //ve toan bo ra
        public override void drawAll()
        {
            poster.showPoster();
            cpu.addInfomation(InfoTable.TYPE.LEVEL, label, ConsoleColor.Yellow);
            cpu.addInfomation(InfoTable.TYPE.STATE, Game.OUG_DESCRIPTION, ConsoleColor.Red);
            transMenuType();
            int size = itemColor.Length;
            for (int i = 0; i < size; i++)
            {
                Console.ForegroundColor = Game.ITEM_SET_COLOR;
                Console.SetCursorPosition(XCL, YCL + i);
                Console.WriteLine(itemColor[i]);
            }
            //item dau tien cua mau sac
            Console.ForegroundColor = color[clY];
            Console.SetCursorPosition(XCL, YCL + clY);
            Console.WriteLine(itemColor[clY]);

            size = itemFigure.Length;
            for (int i = 0; i < size; i++)
            {
                Console.ForegroundColor = Game.ITEM_SET_COLOR;
                Console.SetCursorPosition(XFI, YFI + i);
                Console.WriteLine(itemFigure[i]);

            }
            //item dau tien cua hinh dang
            Console.ForegroundColor = color[clY];
            Console.SetCursorPosition(XFI, YFI + fiY);
            Console.WriteLine(itemFigure[fiY]);

            size = mapColorLable.Length;
            for (int i = 0; i < size; i++)
            {
                Console.ForegroundColor = Game.ITEM_SET_COLOR;
                Console.SetCursorPosition(XCT, YCT + i);
                Console.WriteLine(mapColorLable[i]);
            }

            //item dau tien cua mau ban do
            Console.ForegroundColor = mapColor[ctY];
            Console.SetCursorPosition(XCT, YCT + ctY);
            Console.WriteLine(mapColorLable[ctY]);


            //chi dan
            Console.SetCursorPosition(NOTI1_X, NOTI1_Y);
            Console.ForegroundColor = ConsoleColor.Cyan;
            Console.Write(NOTICE1);
            Console.SetCursorPosition(NOTI2_X, NOTI2_Y);
            Console.Write(NOTICE2);
        }
    }
}
