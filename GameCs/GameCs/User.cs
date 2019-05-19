using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameCs
{
    class User
    {
        int foods;
        int oldScore;
        int mainScore;

        public User()
        {
            foods = oldScore = mainScore = 0;
        }
        public void reset()
        {
            mainScore = oldScore;
            foods = 0;
        }

        public void raise(int size)
        {
            mainScore += size;
            foods++;
        }
        public int getScore
        {
            get
            {
                return mainScore;
            }
        }
        public int getFoods
        {
            get
            {
                return foods;
            }
        }

        public void save()
        {
            foods = 0;
            oldScore = mainScore;

        }
    }
}
