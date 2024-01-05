pipeline {
    agent {
        label 'oot'
    }

    stages {
        stage('Setup') {
            steps {
                sh 'cp /usr/local/etc/roms/baserom_oot.z64 baserom_original.z64'
                sh 'make -j setup'
            }
        }
        stage('Build (qemu-irix)') {
            when {
                branch 'main'
            }
            steps {
                sh 'ORIG_COMPILER=1 make -j'
            }
        }
        stage('Build') {
            when {
                not {
                    branch 'main'
                }
            }
            steps {
                sh 'make -j'
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
