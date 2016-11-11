using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace YMax.Models
{
    public enum YGroupStatus
    {
        UNWRAPPED,
        BAKED
    }

    public enum XRefStatus
    {
        XREF_HIGHT,
        XREF_LOW,
        XREF_LOW_DISPLAY,
        BOX,
        MERGED,
        ORIGINAL,
        FINAL
    }
}
