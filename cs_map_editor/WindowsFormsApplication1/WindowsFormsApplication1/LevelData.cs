using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

//for json convert
using Newtonsoft.Json;
using System.IO;


namespace WindowsFormsApplication1
{
    class LevelData
    {
        public int stage;
        public int w;
        public int h;        
        public TILETYPE[,] ta;
        public static List<LevelData> mapList;

        private const string DATAFILE ="map.json";
        public LevelData(int w, int h, int stage)
        {
            this.stage = stage;
            this.w = w;
            this.h = h;
            ta = new TILETYPE[w, h]; 
            mapList = new List<LevelData>();
        }

        public static void load()
        {
            if(!File.Exists(DATAFILE))
                return;
            mapList = JsonConvert.DeserializeObject<List<LevelData>>(File.ReadAllText(DATAFILE));
        }

        public static void save()
        {
            String str = JsonConvert.SerializeObject(mapList);
            Console.Write(str);
        }
    }
}
