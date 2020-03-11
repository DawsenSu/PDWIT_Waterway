using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.CommandWpf;

namespace PDIWT.Waterway.Lock.UI.Model
{
    public enum ProxyObjectType
    {
        Isolated,
        Continuous
    }
    public enum ProxySolidType
    {
        Isolated_P1,
        Isolated_P11,
        Isolated_P2,
        Isolated_P3,
        Continous_P1
    }
    public class ProxyObjsInfo
    {
        [ReadOnly(true)]
        public long ProxyElementId { get; set; }
        [ReadOnly(true)]
        public ProxyObjectType ProxyType { get; set; }

        public ProxySolidType SolidType { get; set; }

        public string Parameters { get; set; }


    }
}
