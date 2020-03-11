using System;
using System.ComponentModel;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;
using System.Xml.Serialization;
using Xceed.Wpf.Toolkit.PropertyGrid.Attributes;

namespace PDIWT.Waterway.Lock.UI.Model
{
    [Serializable]
    [DisplayName("导航墙P3")]
    [CategoryOrder("断面参数", 0)]
    [CategoryOrder("延路径参数", 1)]
    public class GuidewallP3Data
    {
        private double frontToeHeight = 0.6;
        [Category("断面参数")]
        [DisplayName("前脚趾高(m)")]
        [PropertyOrder(0)]
        public double FrontToeHeight
        {
            get { return frontToeHeight; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("FrontToeHeight", "It should be greater than 0");
                }
                else
                    frontToeHeight = value;
            }
        }

        private double frontToeWidth = 0.3;
        [Category("断面参数")]
        [DisplayName("前脚趾宽(m)")]
        [PropertyOrder(1)]
        public double FrontToeWidth
        {
            get { return frontToeWidth; }
            set
            {
                if (value < 0)
                    throw new ArgumentOutOfRangeException("FrontToeLength", "It should be greater than 0");
                else
                    frontToeWidth = value;
            }
        }

        private double dolphinHeight = 10;
        [Category("断面参数")]
        [DisplayName("墩柱高(m)")]
        [PropertyOrder(2)]
        public double DolphinHeight
        {
            get { return dolphinHeight; }
            set
            {
                if (value < 0)
                    throw new ArgumentOutOfRangeException("DolphinHeight", "It should be greater than 0");
                else
                    dolphinHeight = value;
            }
        }

        private double dolphinTopWidth = 2;
        [Category("断面参数")]
        [DisplayName("墩柱顶宽(m)")]
        [PropertyOrder(3)]
        public double DolphinTopWidth
        {
            get { return dolphinTopWidth; }
            set
            {
                if (value < 0)
                    throw new ArgumentOutOfRangeException("DolphinTopWidth", "It should be greater than 0");
                else
                    dolphinTopWidth = value;
            }
        }
        private double firstBackwallSegAngele = 0;
        [Category("断面参数")]
        [DisplayName("第一折线墙背角度(°)")]
        [PropertyOrder(4)]
        public double FirstBackwallSegAngle
        {
            get { return firstBackwallSegAngele; }
            set
            {
                if (value < 0)
                    throw new ArgumentOutOfRangeException("FirstBackwallSegAngle", "It should be greater than 0");
                else
                    firstBackwallSegAngele = value;
            }
        }
        private double firstBackwallSegHeight = 3;
        [Category("断面参数")]
        [DisplayName("第一折现墙背高度(m)")]
        [PropertyOrder(5)]
        public double FirstBackwallSegHeight
        {
            get { return firstBackwallSegHeight; }
            set
            {
                if (value < 0)
                    throw new ArgumentOutOfRangeException("FirstBackwallSegHeight", "It should be greater than 0");
                else
                    firstBackwallSegHeight = value;
            }
        }
        private double secondBackwallAngle = 45;
        [Category("断面参数")]
        [DisplayName("第二折现墙背角度(°)")]
        [PropertyOrder(6)]
        public double SecondBackwallAngle
        {
            get { return secondBackwallAngle; }
            set
            {
                if (value < 0)
                    throw new ArgumentOutOfRangeException("SecondBackwallAngle", "It should be greater than 0");
                else
                    secondBackwallAngle = value;
            }
        }
        private double interval = 10;
        [Category("延路径参数")]
        [DisplayName("结构段长度(m)")]
        [PropertyOrder(0)]
        public double Interval
        {
            get { return interval; }
            set
            {
                if (value < 0)
                    throw new ArgumentOutOfRangeException("Interval", "It should be greater than 0");
                else
                    interval = value;
            }
        }
        private double gapWidth = 0.02;
        [Category("延路径参数")]
        [DisplayName("施工缝宽度(m)")]
        [PropertyOrder(1)]
        public double GapWidth
        {
            get { return gapWidth; }
            set
            {
                if (value < 0)
                    throw new ArgumentOutOfRangeException("GapWidth", "It should be greater than 0");
                else
                    gapWidth = value;
            }
        }

    }
}
