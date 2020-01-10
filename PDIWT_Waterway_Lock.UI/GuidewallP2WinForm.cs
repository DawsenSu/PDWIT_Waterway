using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PDIWT.Waterway.Lock.UI
{
    public partial class GuidewallP2WinForm
    //: Bentley.MstnPlatformNET.WinForms.Adapter
    : Form
    {
        public GuidewallP2WinForm()
        {
            InitializeComponent();

            //dolphin
            ToggleLabelForeColor(dolphinTopEleTextBox, new Label[]{ dolphinTopEleLabel, dolphinTopEleLabel2 });
            ToggleLabelForeColor(dolphinBottomEleTextBox, new Label[]{ dolphinBottomEleLabel, dolphinBottomEleLabel2 });
            ToggleLabelForeColor(dolphinTopLengthTextBox, new Label[]{ dolphinTopLengthLabel});
            ToggleLabelForeColor(dolphinTopWidthTextBox, new Label[]{ dolphinTopWidthLabel, dolphinTopWidthLabel2 });
            ToggleLabelForeColor(dolphinBottomLengthTextBox, new Label[]{ dolphinBottomLengthLabel});
            ToggleLabelForeColor(dolphinBottomWidthTextBox, new Label[]{ dolphinBottomWidthLabel, dolphinBottomWidthLabel2});
            //cushioncap
            ToggleLabelForeColor(cushioncapBottomEleTextBox, new Label[] { cushioncapBottomEleLabel });
            ToggleLabelForeColor(cushioncapFrontToeLengthTextBox, new Label[]{ cushioncapFrontToeLengthLabel, cushioncapFrontToeLengthLabel2});
            ToggleLabelForeColor(cushioncapRearToeLengthTextBox, new Label[]{ cushioncapRearToeLengthLabel, cushioncapRearToeLengthLabel2});
            ToggleLabelForeColor(cushioncapExtraSideLengthTextBox, new Label[]{ cushioncapExtraSideLengthLabel});
            ToggleLabelForeColor(cushioncapFrontChamferLengthTextBox, new Label[]{ cushioncapFrontChamferLengthLabel});
            //pile
            ToggleLabelForeColor(pileTopEleTextBox, new Label[]{ pileTopEleLabel});
            ToggleLabelForeColor(pileBottomEleTextBox, new Label[] { pileBottomEleLabel });
            //cushion
            ToggleLabelForeColor(cushionThicknesTextBox, new Label[] { cushionThicknessLabel });
            //wall
            ToggleLabelForeColor(wallTopEleTextBox, new Label[] { dolphinTopEleLabel, dolphinTopEleLabel2 });
            ToggleLabelForeColor(wallBottomEleTextBox, new Label[] { wallBottomEleLabel });
            ToggleLabelForeColor(wallThicknessTextBox, new Label[] { wallThicknessLabel });
            ToggleLabelForeColor(wallLengthTextBox, new Label[] { wallLengthLabel });
            //Fender
            
        }

        private Color highlightColor = Color.Red;
        private Color normalColor = Color.Black;

        private void ToggleLabelForeColor(TextBox textBox, Label[] labels)
        {
            textBox.Enter += (s, e) =>
            {
                foreach (var label in labels)
                {
                    label.ForeColor = highlightColor;
                }
            };
            textBox.Leave += (s, e) =>
            {
                foreach (var label in labels)
                {
                    label.ForeColor = normalColor;
                }
            };
        }

    }
}
