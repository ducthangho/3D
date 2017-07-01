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
            prevState.TestStepState = EditStateEnum.Save;
            return prevState;
        }

        public static States EnterStepReducer(States prevState, EnterStepAction action)
        {
            prevState.CurrentStep = action.stepName;
            return prevState;
        }

        public static States ExitStepReducer(States prevState, ExitStepAction action)
        {
            return prevState;
        }

        public static States SwitchStateReducer(States prevState, SwitchStateAction action)
        {
            if (action.NewState == EditStateEnum.Editing)
            {
                if (prevState.TestStepState != EditStateEnum.Ready) return prevState;
            }
            if (action.NewState == EditStateEnum.EndEdit)
            {
                if (prevState.TestStepState != EditStateEnum.Editing) return prevState;
            }
            prevState.TestStepState = action.NewState;
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
            //else if (action is BeginEdit)
            //{
            //    BeginEditReducer(prevState, (BeginEdit)action);
            //}
            //else if (action is EndEditAction)
            //{
            //    EndEditReducer(prevState, (EndEditAction)action);
            //}
            else if (action is SwitchStateAction)
            {
                SwitchStateReducer(prevState, (SwitchStateAction)action);
            }
            return prevState;
        }

    }
}
