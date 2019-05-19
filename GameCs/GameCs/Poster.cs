using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace GameCs
{
    class Poster                                                         
    {
        //Lop poster chua cac menu, anh game,...
        string data;
        
        public Poster(string source)
        {
            FileStream mapdata = null;
            StreamReader tmapdata = null;
            bool isException=false;
            try
            {
                
                mapdata = new FileStream(source, FileMode.Open, FileAccess.Read);
                tmapdata = new StreamReader(mapdata, true);
                data = tmapdata.ReadToEnd().Replace("#", "▓");
            }
            catch (Exception e)
            {
                isException = true;
            }
            finally
            {
                if(mapdata !=null)
                {
                    mapdata.Close();
                }
                if(tmapdata !=null)
                {
                    tmapdata.Close();
                }
                if (isException)
                    Environment.Exit(1);
            }
        }
        public Poster(Poster p)
        {
            this.data = p.data;
        }

        //hien hinh
        public void showPoster()
        {
            Console.ForegroundColor = Game.mapColor;
            Console.SetCursorPosition(0, 0);
            Console.Write(data);
            
        }
    }
}
