properties ([[$class: 'ParametersDefinitionProperty', parameterDefinitions: [
  [$class: 'StringParameterDefinition', name: 'mbed_os_revision', defaultValue: '', description: 'Revision of mbed-os to build. Use format "pull/PR-NUMBER/head" to access mbed-os PR']
  ]]])

if (params.mbed_os_revision == '') {
  echo 'Use mbed OS revision from mbed-os.lib'
} else {
  echo "Use mbed OS revisiong ${params.mbed_os_revision}"
  if (params.mbed_os_revision.matches('pull/\\d+/head')) {
    echo "Revision is a Pull Request"
  }
}

// List of targets with supported RF shields to compile
def targets = [
  "UBLOX_EVK_ODIN_W2",
  "K64F",
  "NUCLEO_F429ZI"
  ]

// Map toolchains to compilers
def toolchains = [
  ARM: "armcc",
  GCC_ARM: "arm-none-eabi-gcc",
  IAR: "IAR-linux"
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

timestamps {
  parallel stepsForParallel
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
