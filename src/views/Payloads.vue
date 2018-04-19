
<template>
  <div class="payloads">
    <h1>Payloads</h1>
    <div class="container">
        <h3>Usages</h3>
        <gauge-bar
            :max-value="info.total"
            :current-value="info.used"
            :value-formatter="numberFormat"
            :show-total="true"
            :show-pct="true">
        </gauge-bar>
        <label class="hints">*There are some spaces allocated for the user interface code</label>
        <h3>Manage</h3>
        <div class="firms">
          <p-radio v-for="f in firmwares" :key="f" :value="f" v-model="selectedFirm" class="p-default" name="firmware" color="warning-o">{{ f }}</p-radio>
          <span v-if="uploadSupported">
            <form ref="form" :action="`${$config.host()}upload`" method="POST" enctype="multipart/form-data">
              <a href="" class="button" @click.prevent="$refs.upload.click()">Upload</a>
              <input type="file" name="upload" ref="upload" @change="uploadFile" />
            </form>
          </span>
          <div class="no-upload" v-else>
            Upload payloads not supported, try Chrome.
          </div>
        </div>
        <table class="payloads-list">
          <payload-info v-for="p in firmPayloads" :key="p.key"
            :payload="p"
            :total-kb="info.total"
            :value-formatter="numberFormat"
            :delete-handler="deletePayload">
          </payload-info>
        </table>
        
    </div>
  </div>
</template>

<script>
import { mapState } from 'vuex';
import GaugeBar from '@/components/GaugeBar';
import PayloadInfo from '@/components/PayloadInfo';

export default {
  name: 'payloads',
  components: {
    [GaugeBar.name]: GaugeBar,
    [PayloadInfo.name]: PayloadInfo,
  },
  data: () => ({
    info: {},
    selectedFirm: '4.55',
  }),
  created() {
    this.$http.getFileSystem(result => {
      this.info = result;
    });
  },
  methods: {
    numberFormat(val) {
      return `${(val / 1024.0).toFixed(2)}Kb`;
    },
    deletePayload(key) {
      console.log(key);
    },
    uploadFile() {
      this.$refs.form.submit();
      // var f = this.$refs.upload.files[0];

      // if (f) {
      //   var r = new FileReader();
      //   r.onload = e => {
      //     var contents = e.target.result;
      //     const payloadName = prompt('New payload name?');
      //     if (payloadName) {
      //       console.log(contents);
      //     } else {
      //       alert('No payload name specified, aborted.');
      //     }

      //     this.$refs.upload.value = null;
      //   };
      //   r.readAsText(f);
      // } else {
      //   alert('Failed to load file');
      // }
    },
  },
  computed: {
    ...mapState(['payloads']),
    firmwares() {
      return this.payloads
        .map(p => p.fw)
        .reduce((x, y) => (x.includes(y) ? x : [...x, y]), []);
    },
    firmPayloads() {
      return this.payloads.filter(p => p.fw === this.selectedFirm);
    },
    uploadSupported() {
      return window.File && window.FileReader && window.FileList && window.Blob;
    },
  },
};
</script>

<style lang="less">
.payloads {
  .container {
    text-align: left;

    .firms {
      padding: 20px;

      form {
        display: inline-block;
      }

      input[type='file'] {
        display: none;
      }

      .no-upload {
        color: #c00;
        padding-top: 20px;
      }
    }
    .payloads-list {
      width: 100%;
    }
  }
}
</style>
