using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PDIWT.Waterway.Lock.UI.ViewModel
{
    using Model;
    public interface ICreateElementByProxyObjs
    {
        List<ProxyObjsInfo> PopulateProxyObjsListFromActiveModel();
        void CreateSolidByProxyObject(List<ProxyObjsInfo> proxyObjInfoList);
    }
}
