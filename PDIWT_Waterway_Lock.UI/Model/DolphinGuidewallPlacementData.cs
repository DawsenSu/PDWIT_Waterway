using System;
using System.ComponentModel;
using System.Text;
using System.Windows;
using System.Reflection;
using System.Xml.Serialization;
using Xceed.Wpf.Toolkit.PropertyGrid.Attributes;

namespace PDIWT.Waterway.Lock.UI.Model
{
    [Serializable]
    public class DolphinGuidewallPlacementData
    {

        private int _proxyType = 0;
        [DisplayName("代理体类型")]
        [ItemsSource(typeof(ProxyObjType))]
        [PropertyOrder(0)]
        [RefreshProperties(RefreshProperties.All)]
        public int ProxyObjType
        {
            get { return _proxyType; }
            set
            {
                _proxyType = value;
                PropertyDescriptor _intervalDescr = TypeDescriptor.GetProperties(this.GetType())["Interval"];
                PropertyDescriptor _numberDescr = TypeDescriptor.GetProperties(this.GetType())["Number"];
                PropertyDescriptor _methodDescr = TypeDescriptor.GetProperties(this.GetType())["Method"];
                BrowsableAttribute _intervalBrowsAttr = (BrowsableAttribute)_intervalDescr.Attributes[typeof(BrowsableAttribute)];
                BrowsableAttribute _numberBrowsAttr = (BrowsableAttribute)_numberDescr.Attributes[typeof(BrowsableAttribute)];
                BrowsableAttribute _methodBrowsAttr = (BrowsableAttribute)_methodDescr.Attributes[typeof(BrowsableAttribute)];
                FieldInfo _intervalBrowsToChange = _intervalBrowsAttr.GetType().GetField("browsable", BindingFlags.NonPublic | BindingFlags.Instance);
                FieldInfo _numberBrowsToChange = _numberBrowsAttr.GetType().GetField("browsable", BindingFlags.NonPublic | BindingFlags.Instance);
                FieldInfo _methodBrowsToChange = _methodBrowsAttr.GetType().GetField("browsable", BindingFlags.NonPublic | BindingFlags.Instance);
                _intervalBrowsToChange.SetValue(_intervalBrowsAttr, _proxyType == 0);
                _numberBrowsToChange.SetValue(_numberBrowsAttr, _proxyType == 0 && _method == 1);
                _methodBrowsToChange.SetValue(_methodBrowsAttr, _proxyType == 0);
            }
        }
        
        private int _method = 0;
        [Browsable(true)]
        [DisplayName("放置方法")]
        [ItemsSource(typeof(ChooseMethods))]
        [PropertyOrder(1)]
        [RefreshProperties(RefreshProperties.All)]
        public int Method
        {
            get { return _method; }
            set
            {
                _method = value;
                PropertyDescriptor _intervalDescr = TypeDescriptor.GetProperties(this.GetType())["Interval"];
                PropertyDescriptor _numberDescr = TypeDescriptor.GetProperties(this.GetType())["Number"];
                BrowsableAttribute _intervalBrowsAttr = (BrowsableAttribute)_intervalDescr.Attributes[typeof(BrowsableAttribute)];
                BrowsableAttribute _numberBrowsAttr = (BrowsableAttribute)_numberDescr.Attributes[typeof(BrowsableAttribute)];
                FieldInfo _intervalBrowsToChange = _intervalBrowsAttr.GetType().GetField("browsable", BindingFlags.NonPublic | BindingFlags.Instance);
                FieldInfo _numberBrowsToChange = _numberBrowsAttr.GetType().GetField("browsable", BindingFlags.NonPublic | BindingFlags.Instance);
                //_intervalBrowsToChange.SetValue(_intervalBrowsAttr, _method == 0 && _proxyType == 0);
                _numberBrowsToChange.SetValue(_numberBrowsAttr, _method == 1 && _proxyType == 0);
            }
        }


        private int m_interval = 20;

        [Browsable(true)]
        [DisplayName("间隔(m)")]
        [UnitScale(Units.Meter)]
        [PropertyOrder(2)]
        public int Interval
        {
            get { return m_interval; }
            set
            {
                if (value <= 0)
                    throw new ArgumentOutOfRangeException("Interval", "It should be not less than 1");
                else
                {
                    m_interval = value;
                }
            }
        }

        private int m_number = 1;

        [Browsable(false)]
        [DisplayName("数量")]
        [UnitScale(Units.Meter)]
        [PropertyOrder(3)]
        public int Number
        {
            get { return m_number; }
            set
            {
                if (value <= 0)
                    throw new ArgumentOutOfRangeException("Number", "It should be not less than 1");
                else
                    m_number = value;
            }
        }
    }
}

public class ChooseMethods : IItemsSource
{
    public ItemCollection GetValues()
    {
        ItemCollection methods = new ItemCollection
        {
            { 0, "按间隔排布" },
            { 1, "按数量排布" }
        };
        return methods;
    }
}

public class ProxyObjType : IItemsSource
{
    public ItemCollection GetValues()
    {
        return new ItemCollection
        {
            {0, "墩式" },
            {1, "重力式" }
        };
    }
}