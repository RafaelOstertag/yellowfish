pipeline {
    agent {
        label 'pi'
    }

    options {
        disableConcurrentBuilds()
        ansiColor('xterm')
        buildDiscarder logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '15')
        timestamps()
    }

    triggers {
        pollSCM '@hourly'
        cron '@daily'
    }

    stages {
        stage('Sonar Analysis') {
            agent {
                label 'amd64&&sonar-buildwrapper'
            }

            stages {
                stage('Configure') {
                    steps {
                        dir('build') {
                            sh 'cmake -DCMAKE_BUILD_TYPE=Release ..'
                        }
                    }
                }
                stage('Build') {
                    steps {
                        dir('build') {
                            sh '/opt/build-wrapper-linux-x86/build-wrapper-linux-x86-64 --out-dir sonar-out cmake --build .'
                        }
                    }
                }
                stage('Checks') {
                    steps {
                        dir('build') {
                            sh 'ctest'
                        }
                    }
                }

                stage('Analyze') {
                    steps {
                        script {
                            sonar_scanner()
                        }
                    }
                }

                stage("Quality Gate") {
                    steps {
                        timeout(time: 30, unit: 'MINUTES') {
                            waitForQualityGate abortPipeline: true
                        }
                    }
                }
            }
        }

        stage('Deploy') {
            when {
                allOf {
                    branch 'master'
                    not {
                        triggeredBy 'TimerTrigger'
                    }
                }
            }

            steps {
                script {
                    step([$class                 : "RundeckNotifier",
                          includeRundeckLogs     : true,
                          jobId                  : "d7101892-0a31-4716-99a1-de3e60570136",
                          options                : "commit=${env.GIT_COMMIT}",
                          rundeckInstance        : "gizmo",
                          shouldFailTheBuild     : true,
                          shouldWaitForRundeckJob: true,
                          tailLog                : true])
                }
            }
             
        }
    }

    post {
        unsuccessful {
            mail to:"rafi@guengel.ch",
            subject:"${JOB_NAME} (${BRANCH_NAME};${env.BUILD_DISPLAY_NAME}) -- ${currentBuild.currentResult}",
            body:"Refer to ${currentBuild.absoluteUrl}"
        }
    }
}

def sonar_scanner() {
    def scannerHome = tool 'Latest SonarQube Scanner';
    withSonarQubeEnv(installationName: 'Sonarcloud', credentialsId: 'e8795d01-550a-4c05-a4be-41b48b22403f') {
        sh label: 'sonarcloud', script: "${scannerHome}/bin/sonar-scanner -Dsonar.branch.name=${env.BRANCH_NAME}"
    }
}
