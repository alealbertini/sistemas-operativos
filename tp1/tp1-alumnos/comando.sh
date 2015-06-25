echo "Corrida para 1 core..."
echo "Quantum 2"
for i in $(seq 1 1000); do ./simusched ejercicio7.tsk 1 1 2 SchedRR 2 2>/dev/null | awk -f filtro.awk; done > cores001_q002.csv;
echo "Quantum 3"
for i in $(seq 1 1000); do ./simusched ejercicio7.tsk 1 1 2 SchedRR 3 2>/dev/null | awk -f filtro.awk; done > cores001_q003.csv;
echo "Quantum 4"
for i in $(seq 1 1000); do ./simusched ejercicio7.tsk 1 1 2 SchedRR 4 2>/dev/null | awk -f filtro.awk; done > cores001_q004.csv;
echo "Quantum 5"
for i in $(seq 1 1000); do ./simusched ejercicio7.tsk 1 1 2 SchedRR 5 2>/dev/null | awk -f filtro.awk; done > cores001_q005.csv;
echo "Quantum 6"
for i in $(seq 1 1000); do ./simusched ejercicio7.tsk 1 1 2 SchedRR 6 2>/dev/null | awk -f filtro.awk; done > cores001_q006.csv;
echo "Quantum 7"
for i in $(seq 1 1000); do ./simusched ejercicio7.tsk 1 1 2 SchedRR 7 2>/dev/null | awk -f filtro.awk; done > cores001_q007.csv;
echo "Quantum 8"
for i in $(seq 1 1000); do ./simusched ejercicio7.tsk 1 1 2 SchedRR 8 2>/dev/null | awk -f filtro.awk; done > cores001_q008.csv;
echo "Quantum 9"
for i in $(seq 1 1000); do ./simusched ejercicio7.tsk 1 1 2 SchedRR 9 2>/dev/null | awk -f filtro.awk; done > cores001_q009.csv;
echo "Quantum 10"
for i in $(seq 1 1000); do ./simusched ejercicio7.tsk 1 1 2 SchedRR 10 2>/dev/null | awk -f filtro.awk; done > cores001_q010.csv;


import sys.process._

implicit def intWithTimes(n: Int) = new {
  def times(f: => Unit) = 1 to n foreach {_ => f}
}

case class RunResults(avgWaitingTime: Double, avgCpuEfficiency: Double)

def getCommandForQuantum(q: Int, cores: Int): String = {
  val qStringBuilder = StringBuilder.newBuilder

  cores times {
    qStringBuilder.append(s" $cores")
  }
  s"./simusched ejercicio7.tsk $cores 1 2 SchedRR${qStringBuilder.toString()}"

}

def runCommand(command: String): RunResults = {
  val result: String = command #| "awk -f filtro.awk" !!

  val split = result.split(";")
  val avgWaitingTime = split.head.toDouble
  val cpuEfficiency: List[Double] = split.tail.map(_.toDouble).toList
  val avgCpuEfficiency = cpuEfficiency.sum / cpuEfficiency.length

  RunResults(avgWaitingTime, avgCpuEfficiency)
}

case class RunResultsForQuantum(q: Int, avgWaitingTime: Double, avgCpuEfficiency: Double)

def runForQuantum(q: Int, cores: Int, iterations: Int): RunResultsForQuantum = {
  print(s"Running for $cores cores, q: $q... \n")
  val command = getCommandForQuantum(q, cores)

  val results: Seq[RunResults] = for {
    it <- 1 to iterations
  } yield {
      print(s"Doing $it/$iterations\n")
      runCommand(command)
  }

  val avgWT = results.map(_.avgWaitingTime).sum / results.size
  val avgCPU = results.map(_.avgCpuEfficiency).sum / results.size

  RunResultsForQuantum(q, avgWT, avgCPU)
}

def runForCores(minQ: Int, maxQ: Int, cores: Int, iterations: Int): Seq[RunResultsForQuantum] = {
  print(s"Running for $cores cores... \n")
  for {
    q <- minQ to maxQ
  } yield runForQuantum(q, cores, iterations)
}

case class RunResultsForCores(cores: Int, resultsForQuantums: Seq[RunResultsForQuantum])

def runAll(minCores: Int, maxCores: Int, minQ: Int, maxQ: Int, iterations: Int): Seq[RunResultsForCores] = {
  for {
    cores <- minCores to maxCores
  } yield RunResultsForCores(cores, runForCores(minQ, maxQ, cores, iterations))
}

def runAndPrintResults(minCores: Int, maxCores: Int, minQ: Int, maxQ: Int, iterations: Int) = {
  val res = runAll(minCores, maxCores, minQ, maxQ, iterations)

  print("==== RESULTS ====\n")
  res.foreach { r =>
    print(s"Cores: ${r.cores}:\n")
    r.resultsForQuantums.foreach{ q =>
      print(s"  q: ${q.q}. Wait: ${q.avgWaitingTime}, CPU: ${q.avgCpuEfficiency} \n")
    }

  }
}