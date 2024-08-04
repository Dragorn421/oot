pipeline {
    agent {
        label 'oot'
    }

    stages {
        stage('Check formatting (full)') {
            when {
                branch 'main'
            }
            steps {
                echo 'Checking formatting on all files...'
                sh 'python3 tools/check_format.py'
            }
        }
        stage('Check formatting (modified)') {
            when {
                not {
                    branch 'main'
                }
            }
            steps {
                echo 'Checking formatting on modified files...'
                sh 'python3 tools/check_format.py --verbose --compare-to origin/main'
            }
        }
        stage('Setup gc-eu-mq-dbg') {
            steps {
                sh 'cp /usr/local/etc/roms/oot-gc-eu-mq-dbg.z64 baseroms/gc-eu-mq-dbg/baserom.z64'
                sh 'make -j setup'
            }
        }
        stage('Build gc-eu-mq-dbg') {
            steps {
                sh 'make -j RUN_CC_CHECK=0'
            }
        }
        stage('Setup gc-eu-mq') {
            steps {
                sh 'cp /usr/local/etc/roms/oot-gc-eu-mq.z64 baseroms/gc-eu-mq/baserom.z64'
                sh 'make -j setup VERSION=gc-eu-mq'
            }
        }
        stage('Build gc-eu-mq') {
            steps {
                sh 'make -j VERSION=gc-eu-mq RUN_CC_CHECK=0'
            }
        }
        stage('Report Progress') {
            when {
                branch 'main'
            }
            steps {
                sh 'mkdir reports'
                sh 'python3 progress.py csv >> reports/progress.csv'
                sh 'python3 progress.py csv -m >> reports/progress_matching.csv'
                sh 'python3 progress.py shield-json > reports/progress_shield.json'
                stash includes: 'reports/*', name: 'reports'
            }
        }
        stage('Update Progress') {
            when {
                branch 'main'
            }
            agent {
                label 'zeldaret_website'
            }
            steps {
                unstash 'reports'
                sh 'cat reports/progress.csv >> /var/www/html/reports/progress.csv'
                sh 'cat reports/progress_matching.csv >> /var/www/html/reports/progress_matching.csv'
                sh 'cat reports/progress_shield.json > /var/www/html/reports/progress_shield.json'
            }
        }
    }
    post {
        always {
            cleanWs()
        }
    }
}
