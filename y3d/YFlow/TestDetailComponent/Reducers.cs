using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using y3d.e;
using Redux;

namespace YFlow.TestDetailComponent
{
    public static class Reducers
    {
        public static States ApplyStepReducer(States prevState, ApplyStepAction action)
        {
            return prevState;
        }

        public static States EnterStepReducer(States prevState, EnterStepAction action)
        {
            return prevState;
        }

        public static States ExitStepReducer(States prevState, ExitStepAction action)
        {
            return prevState;
        }

        public static States TestDetailReducer(States prevState, IAction action)
        {
            if (action is ApplyStepAction)
            {
                ApplyStepReducer(prevState, (ApplyStepAction)action);
            }
            else if (action is ExitStepAction)
            {
                ExitStepReducer(prevState, (ExitStepAction)action);
            }
            else if (action is EnterStepAction)
            {
                EnterStepReducer(prevState, (EnterStepAction)action);
            }
            return prevState;
        }

    }
}
