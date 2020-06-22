properties ([[$class: 'ParametersDefinitionProperty', parameterDefinitions: [
  [$class: 'StringParameterDefinition', name: 'mbed_os_revision', defaultValue: '', description: 'Revision of mbed-os to build. Use format "pull/PR-NUMBER/head" to access mbed-os PR'],
  [$class: 'BooleanParameterDefinition', name: 'smoke_test', defaultValue: true, description: 'Runs HW smoke test with eaxmple']
  ]]])

if (env.MBED_OS_REVISION == null) {
  echo 'First run in this branch, using default parameter values'
  env.MBED_OS_REVISION = ''
}
if (env.MBED_OS_REVISION == '') {
  echo 'Using mbed OS revision from mbed-os.lib'
} else {
  echo "Using given mbed OS revision: ${env.MBED_OS_REVISION}"
  if (env.MBED_OS_REVISION.matches('pull/\\d+/head')) {
    echo "Revision is a Pull Request"
  }
}

// Map RaaS instances to corresponding test suites
def raas = [
  "sockets_smoke_k64f.json": "ruka"
]

// List of targets with supported RF shields to compile
def targets = [
  "K64F",
  "NUCLEO_F429ZI"
  ]

// Map toolchains to compilers
def toolchains = [
  ARM: "armcc",
  GCC_ARM: "arm-none-eabi-gcc"
  ]

def stepsForParallel = [:]

// Jenkins pipeline does not support map.each, we need to use oldschool for loop
for (int i = 0; i < targets.size(); i++) {
    for(int j = 0; j < toolchains.size(); j++) {
          def target = targets.get(i)
          def toolchain = toolchains.keySet().asList().get(j)
          def compilerLabel = toolchains.get(toolchain)

          def stepName = "${target} ${toolchain}"
          stepsForParallel[stepName] = buildStep(target, compilerLabel, toolchain)
    }
}

def parallelRunSmoke = [:]

// Need to compare boolean against string value
if (params.smoke_test == true) {
  echo "Running smoke tests"
  // Generate smoke tests based on suite amount
  for(int i = 0; i < raas.size(); i++) {
    def suite_to_run = raas.keySet().asList().get(i)
    def raasName = raas.get(suite_to_run)

    // Parallel execution needs unique step names. Remove .json file ending.
    def smokeStep = "${raasName} ${suite_to_run.substring(0, suite_to_run.indexOf('.'))}"
    parallelRunSmoke[smokeStep] = run_smoke(raasName, suite_to_run, toolchains, targets)
  }
} else {
  echo "Skipping smoke tests"
}

timestamps {
  parallel stepsForParallel
  parallel parallelRunSmoke
}

def buildStep(target, compilerLabel, toolchain) {
  return {
    stage ("${target}_${compilerLabel}") {
      node ("${compilerLabel}") {
        deleteDir()
        dir("mbed-os-example-socket") {
          checkout scm

          // Set mbed-os to revision received as parameter
          execute ("mbed deploy --protocol ssh")
          if (params.mbed_os_revision != '') {
            dir ("mbed-os") {
              if (params.mbed_os_revision.matches('pull/\\d+/head')) {
                execute("git fetch origin ${params.mbed_os_revision}:PR")
                execute("git checkout PR")
              } else {
                execute ("git checkout ${params.mbed_os_revision}")
              }
            }
          }
          execute("mbed new .")
          execute ("mbed compile --build out/${target}_${toolchain}/ -m ${target} -t ${toolchain}")
        }
        stash name: "${target}_${toolchain}", includes: '**/mbed-os-example-socket.bin'
        archive '**/mbed-os-example-socket.bin'
        step([$class: 'WsCleanup'])
      }
    }
  }
}

def run_smoke(raasName, suite_to_run, toolchains, targets) {
  return {
    env.RAAS_USERNAME = "ci"
    env.RAAS_PASSWORD = "ci"
    // Remove .json from suite name
    def suiteName = suite_to_run.substring(0, suite_to_run.indexOf('.'))
    stage ("smoke_${raasName}_${suiteName}") {
      //node is actually the type of machine, i.e., mesh-test boild down to linux
      node ("linux") {
        deleteDir()
        dir("mbed-clitest") {
          git "git@github.com:ARMmbed/mbed-clitest.git"
          execute("git checkout ${env.LATEST_CLITEST_STABLE_REL}")
          dir("mbed-clitest-suites") {
            git "git@github.com:ARMmbed/mbed-clitest-suites.git"
            dir("cellular") {
              git "git@github.com:ARMmbed/mbed-clitest-cellular.git"
            }
          }

          for (int i = 0; i < targets.size(); i++) {
            for(int j = 0; j < toolchains.size(); j++) {
              def target = targets.get(i)
              def toolchain = toolchains.keySet().asList().get(j)
              unstash "${target}_${toolchain}"
            }
          }
          execute("python clitest.py --suitedir mbed-clitest-suites/suites/ --suite ${suite_to_run} --type hardware --reset \
                  --raas https://${raasName}.mbedcloudtesting.com:443 --tcdir mbed-clitest-suites/cellular --raas_queue --raas_queue_timeout 3600 \
                  --raas_share_allocs --failure_return_value -v -w --log log_${raasName}_${suiteName}")
          archive "log_${raasName}_${suiteName}/**/*"
        }
      }
    }
  }
}
