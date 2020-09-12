pipeline {
    agent {
        label 'pi'
    }

    options {
        disableConcurrentBuilds()
        ansiColor('xterm')
        buildDiscarder logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '10')
        timestamps()
    }

    triggers {
        cron '@daily'
    }

    stages {
        stage('Configure') {
            steps {
                dir('build') {
                    sh 'rm -rf *'
                    sh 'cmake -DCMAKE_BUILD_TYPE=Release ..'
                }
            }
        }
        stage('Build') {
            steps {
                dir('build') {
                    sh 'cmake --build .'
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
