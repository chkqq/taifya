﻿namespace MealyMooreMinimization
{
    internal class MooreMachineInfo : IMinimizableMachineInfo
    {
        public MooreMachineInfo(List<string> info = null)
        {
            OutputAlphabet = new List<string>();
            States = new List<string>();
            InputAlphabet = new List<string>();
            TransitionFunctions = new List<List<string>>();

            if (info == null || info.Count == 0)
            {
                return;
            }

            string[] dirtyOutputAlphabet = info[0].Split(";");
            OutputAlphabet = new ArraySegment<string>(dirtyOutputAlphabet, 1, dirtyOutputAlphabet.Length - 1).ToList();
            string[] dirtyStates = info[1].Split(";");
            States = new ArraySegment<string>(dirtyStates, 1, dirtyStates.Length - 1).ToList();
            for (int i = 2; i < info.Count; i++)
            {
                string[] values = info[i].Split(";");
                InputAlphabet.Add(values[0]);
                TransitionFunctions.Add(new ArraySegment<string>(values, 1, values.Length - 1).ToList());
            }

            if (States.Count <= 1)
            {
                throw new ArgumentException("Incorrect input machine. Number of states can not be less than 2");
            }
        }

        public List<string> States;
        public List<List<string>> TransitionFunctions;
        public List<string> OutputAlphabet;
        public List<string> InputAlphabet;

        public string GetCsvData()
        {
            string csvData = "";

            csvData += ";";
            csvData += this.OutputAlphabet.Aggregate((total, part) => $"{total};{part}");
            csvData += "\n";

            csvData += ";";
            csvData += this.States.Aggregate((total, part) => $"{total};{part}");
            csvData += "\n";

            for (int i = 0; i < this.InputAlphabet.Count; i++)
            {
                csvData += this.InputAlphabet[i] + ";";
                csvData += this.TransitionFunctions[i].Aggregate((total, part) => $"{total};{part}");
                csvData += "\n";
            }

            return csvData;
        }

        public void Minimize()
        {
            DeleteUnreachableStates();
            Dictionary<string, HashSet<string>> previousMatchingMinimizedStatesToStates = new Dictionary<string, HashSet<string>>();
            Dictionary<string, HashSet<string>> matchingMinimizedStatesToStates = new Dictionary<string, HashSet<string>>();
            foreach (string outputLetter in OutputAlphabet.ToHashSet())
            {
                HashSet<string> outputLetterStates = new HashSet<string>();
                for (int i = 0; i < OutputAlphabet.Count; i++)
                {
                    if (OutputAlphabet[i] == outputLetter)
                    {
                        outputLetterStates.Add(States[i]);
                    }
                }
                matchingMinimizedStatesToStates.Add(outputLetter, outputLetterStates);
            }
            List<List<string>> newTransitionFunctions = GetNewTransitionFunctions(matchingMinimizedStatesToStates, TransitionFunctions);

            do
            {
                previousMatchingMinimizedStatesToStates = matchingMinimizedStatesToStates;
                matchingMinimizedStatesToStates = GetMatchingMinimizedStatesToStates(previousMatchingMinimizedStatesToStates, newTransitionFunctions);
                newTransitionFunctions = GetNewTransitionFunctions(matchingMinimizedStatesToStates, TransitionFunctions);
            } while (matchingMinimizedStatesToStates.Count != previousMatchingMinimizedStatesToStates.Count && States.Count != matchingMinimizedStatesToStates.Count);

            List<string> minimizedStates = matchingMinimizedStatesToStates.Keys.ToList();
            List<List<string>> minimizedTransitionFunctions = GetMinimizedTransitionFunctions(matchingMinimizedStatesToStates);
            List<string> minimizedOutputAlphabet = GetMinimizedOutputAlphabet(matchingMinimizedStatesToStates);
            States = minimizedStates;
            TransitionFunctions = minimizedTransitionFunctions;
            OutputAlphabet = minimizedOutputAlphabet;
        }

        private void DeleteUnreachableStates()
        {
            HashSet<string> reachableStates = new HashSet<string>();

            // Добавляем начальное состояние как достижимое
            reachableStates.Add(States[0]);

            // Используем очередь для обхода всех достижимых состояний
            Queue<string> queue = new Queue<string>();
            queue.Enqueue(States[0]);

            while (queue.Count > 0)
            {
                string currentState = queue.Dequeue();
                int currentStateIndex = States.IndexOf(currentState);

                foreach (var transition in TransitionFunctions)
                {
                    if (currentStateIndex >= 0 && currentStateIndex < transition.Count)
                    {
                        string nextState = transition[currentStateIndex];
                        if (!reachableStates.Contains(nextState) && States.Contains(nextState))
                        {
                            reachableStates.Add(nextState);
                            queue.Enqueue(nextState);
                        }
                    }
                }
            }

            // Удаляем недостижимые состояния
            List<string> removedStates = States.Except(reachableStates).ToList();
            foreach (var state in removedStates)
            {
                int index = States.IndexOf(state);
                States.RemoveAt(index);
                OutputAlphabet.RemoveAt(index);
                foreach (var transition in TransitionFunctions)
                {
                    if (index < transition.Count)
                    {
                        transition.RemoveAt(index);
                    }
                }
            }
        }


        private Dictionary<string, HashSet<string>> GetMatchingMinimizedStatesToStates(Dictionary<string, HashSet<string>> matchingEquivalenceClassesToStates, List<List<string>> transitionFunctions)
        {
            Dictionary<string, HashSet<string>> matchingNewStatesToPreviousStates = new Dictionary<string, HashSet<string>>();
            Dictionary<string, List<string>> matchingNewStatesToTransitionFunctions = new Dictionary<string, List<string>>();

            for (int i = 0; i < States.Count; i++)
            {
                List<string> transitionsSequence = new List<string>();
                foreach (List<string> innerStateTransitionFunctions in transitionFunctions)
                {
                    //TODO Change
                    transitionsSequence.Add(innerStateTransitionFunctions[i]);
                }
                bool isExistMinimizedState = false;
                foreach (KeyValuePair<string, List<string>> matchingNewStateToTransitionFunctions in matchingNewStatesToTransitionFunctions)
                {
                    if (matchingNewStateToTransitionFunctions.Value.SequenceEqual(transitionsSequence))
                    {
                        string firstElementOfEquivalenceClass = matchingNewStatesToPreviousStates[matchingNewStateToTransitionFunctions.Key].First();
                        string firstEquivalenceClass = "";
                        string secondEquivalenceClass = "";
                        foreach (KeyValuePair<string, HashSet<string>> matchingEquivalenceClassToStates in matchingEquivalenceClassesToStates)
                        {
                            if (matchingEquivalenceClassToStates.Value.Contains(firstElementOfEquivalenceClass))
                            {
                                firstEquivalenceClass = matchingEquivalenceClassToStates.Key;
                            }
                            if (matchingEquivalenceClassToStates.Value.Contains(States[i]))
                            {
                                secondEquivalenceClass = matchingEquivalenceClassToStates.Key;
                            }
                        }
                        if (firstEquivalenceClass == secondEquivalenceClass)
                        {
                            matchingNewStatesToPreviousStates[matchingNewStateToTransitionFunctions.Key].Add(States[i]);
                            isExistMinimizedState = true;
                            break;
                        }
                    }
                }
                if (!isExistMinimizedState)
                {
                    string newState = "q" + matchingNewStatesToPreviousStates.Count.ToString();
                    matchingNewStatesToPreviousStates.Add(newState, new HashSet<string>() { States[i] });
                    matchingNewStatesToTransitionFunctions.Add(newState, transitionsSequence);
                }
            }

            return matchingNewStatesToPreviousStates;
        }

        private List<List<string>> GetNewTransitionFunctions(Dictionary<string, HashSet<string>> matchingNewStatesToStates, List<List<string>> oldTransitionFunctions)
        {
            List<List<string>> newTransitionFunctions = new List<List<string>>();

            foreach (var transitionRow in oldTransitionFunctions)
            {
                List<string> newRow = new List<string>();
                foreach (var state in transitionRow)
                {
                    string newState = matchingNewStatesToStates
                        .FirstOrDefault(pair => pair.Value.Contains(state)).Key;
                    newRow.Add(newState ?? "");
                }
                newTransitionFunctions.Add(newRow);
            }

            return newTransitionFunctions;
        }


        private List<List<string>> GetMinimizedTransitionFunctions(Dictionary<string, HashSet<string>> matchingMinimizedStatesToStates)
        {
            List<List<string>> minimizedTransitionFunctions = new List<List<string>>();

            foreach (List<string> innerStateTransitionFunctions in TransitionFunctions)
            {
                List<string> innerStateMinimizedTransitionFunctions = new List<string>();
                foreach (string innerStateTransitionFunction in innerStateTransitionFunctions.GetRange(0, matchingMinimizedStatesToStates.Count()))
                {
                    if (innerStateTransitionFunction == "")
                    {
                        innerStateMinimizedTransitionFunctions.Add("");
                    }
                    else
                    {
                        foreach (KeyValuePair<string, HashSet<string>> localMatchingMinimizedStateToStates in matchingMinimizedStatesToStates)
                        {
                            if (localMatchingMinimizedStateToStates.Value.Contains(innerStateTransitionFunction))
                            {
                                innerStateMinimizedTransitionFunctions.Add(localMatchingMinimizedStateToStates.Key);
                            }
                        }
                    }
                }
                minimizedTransitionFunctions.Add(innerStateMinimizedTransitionFunctions);
            }
            return minimizedTransitionFunctions;
        }

        private List<string> GetMinimizedOutputAlphabet(Dictionary<string, HashSet<string>> matchingMinimizedStatesToStates)
        {
            List<string> minimizedOutputAlphabet = new List<string>();

            foreach (KeyValuePair<string, HashSet<string>> matchingMinimizedStateToStates in matchingMinimizedStatesToStates)
            {
                minimizedOutputAlphabet.Add(OutputAlphabet[States.IndexOf(matchingMinimizedStateToStates.Value.First())]);
            }

            return minimizedOutputAlphabet;
        }
    }
}
