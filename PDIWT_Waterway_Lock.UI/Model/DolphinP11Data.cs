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
    [DisplayName("靠船墩P11")]
    [CategoryOrder("墩柱", 0)]
    [CategoryOrder("承台", 1)]
    [CategoryOrder("垫层", 2)]
    [ECClass("PDIWT_Waterway", "DolphinP11")]
    public class DolphinP11Data
    {
        [Browsable(false)]
        [XmlAttribute]
        public string Name { get; set; } = "";

        [Category("墩柱")]
        [DisplayName("顶高程(m)")]
        [PropertyOrder(0)]
        [ECProperty("Parameters.DolphinColumnTopElevation")]
        [UnitScale(Units.Meter)]
        public double DolphinColumnTopElevation { get; set; } = 10;

        [Category("墩柱")]
        [DisplayName("底高程(m)")]
        [PropertyOrder(1)]
        [ECProperty("Parameters.DolphinColumnBottomElevation")]
        [UnitScale(Units.Meter)]
        public double DolphinColumnBottomElevation { get; set; } = 0;

        double m_dolphinColumnTopLength = 3.5;

        [Category("墩柱")]
        [DisplayName("顶长(m)")]
        [PropertyOrder(2)]
        [ECProperty("Parameters.DolphinColumnTopLength")]
        [UnitScale(Units.Meter)]
        public double DolphinColumnTopLength
        {
            get { return m_dolphinColumnTopLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinColumnTopLength", "It should be greater than 0");
                }
                else
                    m_dolphinColumnTopLength = value;
            }
        }

        double m_dolphinColumnTopWidth = 3.5;

        [Category("墩柱")]
        [DisplayName("顶宽(m)")]
        [PropertyOrder(3)]
        [ECProperty("Parameters.DolphinColumnTopWidth")]
        [UnitScale(Units.Meter)]
        public double DolphinColumnTopWidth
        {
            get { return m_dolphinColumnTopWidth; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinColumnTopWidth", "It should be greater than 0");
                }
                else
                    m_dolphinColumnTopWidth = value;
            }
        }

        double m_dolphinColumnTopEdgeRadius = 0.1;

        [Category("墩柱")]
        [DisplayName("顶边倒角半径(m)")]
        [PropertyOrder(4)]
        [ECProperty("Parameters.DolphinColumnTopEdgeRadius")]
        [UnitScale(Units.Meter)]
        public double DolphinColumnTopEdgeRadius
        {
            get { return m_dolphinColumnTopEdgeRadius; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinColumnTopEdgeRadius", "It should be greater than 0");
                }
                else
                    m_dolphinColumnTopEdgeRadius = value;
            }
        }

        double m_dolphinColumnSideEdgeRadius = 0.3;

        [Category("墩柱")]
        [DisplayName("侧边倒角半径(m)")]
        [PropertyOrder(5)]
        [ECProperty("Parameters.DolphinColumnSideEdgeRadius")]
        [UnitScale(Units.Meter)]
        public double DolphinColumnSideEdgeRadius
        {
            get { return m_dolphinColumnSideEdgeRadius; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinColumnSideEdgeRadius", "It should be greater than 0");
                }
                else
                    m_dolphinColumnSideEdgeRadius = value;
            }
        }

        double m_dolphinColumnBottomLength = 3.5;

        [Category("墩柱")]
        [DisplayName("底长(m)")]
        [PropertyOrder(6)]
        [ECProperty("Parameters.DolphinColumnBottomLength")]
        [UnitScale(Units.Meter)]
        public double DolphinColumnBottomLength
        {
            get { return m_dolphinColumnBottomLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinColumnBottomLength", "It should be greater than 0");
                }
                else
                    m_dolphinColumnBottomLength = value;
            }
        }

        double m_dolphinColumnBottomWidth = 5;

        [Category("墩柱")]
        [DisplayName("底宽(m)")]
        [PropertyOrder(7)]
        [ECProperty("Parameters.DolphinColumnBottomWidth")]
        [UnitScale(Units.Meter)]
        public double DolphinColumnBottomWidth
        {
            get { return m_dolphinColumnBottomWidth; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinColumnBottomWidth", "It should be greater than 0");
                }
                else
                    m_dolphinColumnBottomWidth = value;
            }
        }

        double m_dolphinColumnHaunchHeight = 1;

        [Category("墩柱")]
        [DisplayName("底部倒角高度(m)")]
        [PropertyOrder(8)]
        [ECProperty("Parameters.DolphinColumnHaunchHeight")]
        [UnitScale(Units.Meter)]
        public double DolphinColumnHaunchHeight
        {
            get { return m_dolphinColumnHaunchHeight; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinColumnHaunchHeight", "It should be greater than 0");
                }
                else
                    m_dolphinColumnHaunchHeight = value;
            }
        }

        double m_dolphinColumnHaunchLength = 0.5;

        [Category("墩柱")]
        [DisplayName("底部倒角长度(m)")]
        [PropertyOrder(9)]
        [ECProperty("Parameters.DolphinColumnHaunchLength")]
        [UnitScale(Units.Meter)]
        public double DolphinColumnHaunchLength
        {
            get { return m_dolphinColumnHaunchLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("DolphinColumnHaunchLength", "It should be greater than 0");
                }
                else
                    m_dolphinColumnHaunchLength = value;
            }
        }

        double m_angleOfFirstPolylineWall = 0;

        [Category("墩柱")]
        [DisplayName("第一折线墙背角度(°)")]
        [PropertyOrder(10)]
        [ECProperty("Parameters.AngleOfFirstPolylineWall")]
        [UnitScale(Units.Meter)]
        public double AngleOfFirstPolylineWall
        {
            get { return m_angleOfFirstPolylineWall; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("AngleOfFirstPolylineWall", "It should be greater than 0");
                }
                else
                    m_angleOfFirstPolylineWall = value;
            }
        }

        double m_heightOfFirstPolylineWall = 4;

        [Category("墩柱")]
        [DisplayName("第一折线墙背高度(m)")]
        [PropertyOrder(11)]
        [ECProperty("Parameters.HeightOfFirstPolylineWall")]
        [UnitScale(Units.Meter)]
        public double HeightOfFirstPolylineWall
        {
            get { return m_heightOfFirstPolylineWall; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("HeightOfFirstPolylineWall", "It should be greater than 0");
                }
                else
                    m_heightOfFirstPolylineWall = value;
            }
        }

        [Category("承台")]
        [DisplayName("底高程(m)")]
        [PropertyOrder(0)]
        [ECProperty("Parameters.CushionCapBottomElevation")]
        [UnitScale(Units.Meter)]
        public double CushionCapBottomElevation { get; set; } = -1.5;

        double m_frontToeLength = 1;

        [Category("承台")]
        [DisplayName("前趾长(m)")]
        [PropertyOrder(1)]
        [ECProperty("Parameters.FrontToeLength")]
        [UnitScale(Units.Meter)]
        public double FrontToeLength
        {
            get { return m_frontToeLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("FrontToeLength", "It should be greater than 0");
                }
                else
                    m_frontToeLength = value;
            }
        }

        double m_backToeLength = 1;

        [Category("承台")]
        [DisplayName("后趾长(m)")]
        [PropertyOrder(2)]
        [ECProperty("Parameters.BackToeLength")]
        [UnitScale(Units.Meter)]
        public double BackToeLength
        {
            get { return m_backToeLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("BackToeLength", "It should be greater than 0");
                }
                else
                    m_backToeLength = value;
            }
        }

        double m_cushionCapExtraWideLength = 1;

        [Category("承台")]
        [DisplayName("两侧超宽(m)")]
        [PropertyOrder(3)]
        [ECProperty("Parameters.CushionCapExtraWideLength")]
        [UnitScale(Units.Meter)]
        public double CushionCapExtraWideLength
        {
            get { return m_cushionCapExtraWideLength; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("CushionCapExtraWideLength", "It should be greater than 0");
                }
                else
                    m_cushionCapExtraWideLength = value;
            }
        }

        double m_capThickness = 100;

        [Category("垫层")]
        [DisplayName("厚度(mm)")]
        [ECProperty("Parameters.CapThickness")]
        [UnitScale(Units.Millimeter)]
        public double CapThickness
        {
            get { return m_capThickness; }
            set
            {
                if (value < 0)
                {
                    throw new ArgumentOutOfRangeException("CapThickness", "It should be greater than 0");
                }
                else
                    m_capThickness = value;
            }
        }

        public bool ValidateParameters()
        {
            if (DolphinColumnBottomElevation >= DolphinColumnTopElevation
                || CushionCapBottomElevation >= DolphinColumnTopElevation)
                return false;
            return true;
        }

        public DolphinP11Data CloneDolphinP11DataInMeters()
        {
            DolphinP11Data data = (DolphinP11Data)MemberwiseClone();
            double scale = 1;
            foreach (var prop in data.GetType().GetProperties())
            {
                UnitScaleAttribute unitScaleAttri = prop.GetCustomAttribute<UnitScaleAttribute>();
                if (null == unitScaleAttri) continue;
                scale = unitScaleAttri.ToMeterScale();
                if (typeof(double) == prop.PropertyType)
                    prop.SetValue(data, (double)prop.GetValue(data) * scale);
                else if (typeof(int) == prop.PropertyType)
                    prop.SetValue(data, (int)prop.GetValue(data) * scale);
                else if (typeof(long) == prop.PropertyType)
                    prop.SetValue(data, (long)prop.GetValue(data) * scale);
            }
            return data;
        }
    }
}
