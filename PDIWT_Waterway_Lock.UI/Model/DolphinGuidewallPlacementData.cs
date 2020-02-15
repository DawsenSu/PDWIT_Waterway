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
        [DisplayName("放置方法")]
        [ItemsSource(typeof(ChooseMethods))]
        public int Method { get; set; }

        private int m_interval = 20;

        [DisplayName("间隔(m)")]
        [UnitScale(Units.Meter)]
        public int Interval
        {
            get { return m_interval; }
            set
            {
                if (value <= 0)
                    throw new ArgumentOutOfRangeException("Interval", "It should be not less than 1");
                else
                    m_interval = value;
            }
        }

        private int m_number =1;
        [DisplayName("数量(m)")]
        [UnitScale(Units.Meter)]
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
        ItemCollection methods = new ItemCollection();
        methods.Add(0, "按间隔排布");
        methods.Add(1, "按数量排布");
        return methods;
    }
}