﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace PDIWT.Waterway.Lock.UI
{
    /// <summary>
    /// Interaction logic for GuidewalP2View.xaml
    /// </summary>
    public partial class GuidewallP2View : UserControl
    {
        public GuidewallP2View()
        {
            InitializeComponent();
            DataContext = new ViewModel.GuidewallP2ViewModel();
        }
    }
}
